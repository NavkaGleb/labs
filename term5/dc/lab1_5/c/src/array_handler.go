package main

import (
	"fmt"
	"math/rand"
	"time"
)

var (
	arrayHandlerId = 0
	random = rand.New(rand.NewSource(time.Now().UnixNano()))
)

func getRandomArray() [elementCount]int {
	a := [elementCount]int{}

	for i := range a {
		a[i] = random.Intn(maxElementValue)
	}

	return a
}

type ArrayHandler struct {
	id 		int
	data 	[elementCount]int
	in     	chan int
	out 	chan int
	isDone 	*bool
}

func newArrayHandler(in, out chan int, isDone *bool) *ArrayHandler {
	arrayHandlerId++

	return &ArrayHandler{
		id: 	arrayHandlerId,
		data: 	getRandomArray(),
		in: 	in,
		out: 	out,
		isDone: isDone,
	}
}

func (h* ArrayHandler) run() {
	for !(*h.isDone) {
		localSum := h.getSum()

		fmt.Printf("%v: %v\n", h.id, h.data)

		h.out <- localSum

		globalAverage := <-h.in

		index := random.Intn(len(h.data))

		switch {
		case globalAverage < localSum:
			h.data[index]--
		case globalAverage > localSum:
			h.data[index]++
		}
	}
}

func (h* ArrayHandler) getSum() int {
	sum := 0

	for _, v := range h.data {
		sum += v
	}

	return sum
}
