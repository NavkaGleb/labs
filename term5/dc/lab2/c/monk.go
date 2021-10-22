package main

import "fmt"

type Monk struct {
	Chi 		int
	Monastery 	Monastery
}

func (monk *Monk) String() string {
	return fmt.Sprintf("%s %d", monk.Monastery, monk.Chi)
}

type MonkContainer []*Monk

