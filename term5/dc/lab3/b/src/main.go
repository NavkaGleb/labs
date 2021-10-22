package main

import (
	"math/rand"
	"time"
)

func main() {
	rand.Seed(time.Now().UnixNano())

	const clientCount = 10

	waitingRoom := NewWaitingRoom()
	barber := NewBarber()

	go barber.Run(waitingRoom)

	for i := 0; i < clientCount; i++ {
		time.Sleep(time.Millisecond * time.Duration(rand.Intn(1500) + 1500))
		go NewClient(ClientId(i)).Run(waitingRoom, barber)
	}

	time.Sleep(time.Second * 5)
}