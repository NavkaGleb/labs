package main

import (
	"container/list"
	"fmt"
	"sync"
)

type WaitingRoom struct {
	Cond	*sync.Cond
	Queue	*list.List
}

func (room *WaitingRoom) Lock() {
	room.Cond.L.Lock()
}

func (room *WaitingRoom) Unlock() {
	room.Cond.L.Unlock()
}

func (room *WaitingRoom) IsEmpty() bool {
	return room.Queue.Len() == 0
}

func (room *WaitingRoom) AddClient(id ClientId) {
	room.Lock()
	{
		fmt.Printf(TerminalGreen("Client #%v came!\n"), id)
		room.Queue.PushBack(id)
		room.Cond.Signal()
	}
	room.Unlock()
}

func (room *WaitingRoom) RemoveClient() {
	fmt.Printf(TerminalRed("Client #%v left!\n"), room.Queue.Front().Value)
	room.Queue.Remove(room.Queue.Front())
}

func NewWaitingRoom() *WaitingRoom {
	return &WaitingRoom{
		Cond: sync.NewCond(&sync.Mutex{}),
		Queue: list.New(),
	}
}