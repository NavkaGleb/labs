package main

import "fmt"

const (
	arrayHandlerCount 	= 3
	elementCount 		= 15
	maxElementValue 	= 20
)

func main() {
	isDone := false

	sendingChannel := make(chan int, 3)
	receivingChannel := make(chan int, 3)

	handlers := make([]*ArrayHandler, 3)

	for i := range handlers {
		handlers[i] = newArrayHandler(sendingChannel, receivingChannel, &isDone)
		go handlers[i].run()
	}

	for {
		sum1 := <-receivingChannel
		sum2 := <-receivingChannel
		sum3 := <-receivingChannel

		fmt.Printf("%v, %v, %v\n\n", sum1, sum2, sum3)

		averageSum := (sum1 + sum2 + sum3) / 3

		for i := 0; i < arrayHandlerCount; i++ {
			sendingChannel <- averageSum
		}

		if sum1 == sum2 && sum1 == sum3 {
			isDone = true
			return
		}
	}
}
