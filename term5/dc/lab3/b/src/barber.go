package main

import (
	"fmt"
	"sync"
	"time"
)

type Barber struct {
	IsSleep bool
	IsFree  *sync.Cond
}

func (barber *Barber) Sleep() {
	barber.IsSleep = true
	fmt.Println("\tBarber went sleep")
}

func (barber *Barber) WakeUp() {
	barber.IsSleep = false
	fmt.Println("\tBarber wake up")
}

func (barber *Barber) WaitForClients(room *WaitingRoom) {
	barber.Sleep()
	room.Cond.Wait()
	barber.WakeUp()
}

func (barber *Barber) MakeHaircut() {
	fmt.Println("\tBarber start making haircut")
	time.Sleep(time.Second * 2)
	fmt.Println("\tBarber finish making haircut")

	barber.IsFree.Signal()
}

func (barber *Barber) Run(room *WaitingRoom) {
	for {
		room.Lock()

		for room.IsEmpty() {
			barber.WaitForClients(room)
		}

		room.Unlock()
		barber.MakeHaircut()
		room.RemoveClient()
	}
}

func NewBarber() *Barber {
	return &Barber{
		IsSleep: false,
		IsFree: sync.NewCond(&sync.Mutex{}),
	}
}
