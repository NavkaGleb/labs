package main

import (
	"fmt"
	"github.com/yourbasic/graph"
	"math/rand"
	"sync"
	"time"
)

const maxGraphSize = 40
var   graphSize    = 4

func getRandNodes(size int) (i, j int) {
	i = 0
	j = 0

	for i == j {
		i = rand.Intn(size)
		j = rand.Intn(size)
	}

	return i, j
}

func modifyEdges(g *graph.Mutable, l *sync.RWMutex) {
	for {
		time.Sleep(time.Millisecond * time.Duration(500 + rand.Intn(1000)))

		l.Lock()
		fmt.Printf(ConsoleColorTeal("%-20s Locked\n"), "[ModifyEdges]:")

		if g.Order() < 2 {
			fmt.Printf(ConsoleColorTeal("%-20s Unlocked\n"), "[ModifyEdges]:")
			l.Unlock()
			continue
		}

		from, to := getRandNodes(g.Order())

		if rand.Intn(2) == 0 {
			var cost = int64(rand.Intn(9) + 1)

			fmt.Printf(ConsoleColorTeal("%-20s Update edge from %d to %d. Cost = %d\n"), "[ModifyEdges]:", from, to, cost)
			g.AddBothCost(from, to, cost)
		} else if g.Edge(from, to) {
			fmt.Printf(ConsoleColorTeal("%-20s Delete edge from %d to %d\n"), "[ModifyEdges]:", from, to)
			g.DeleteBoth(from, to)
		}

		fmt.Printf(ConsoleColorTeal("%-20s Unlocked\n"), "[ModifyEdges]:")
		l.Unlock()
	}
}

func modifyNodes(g *graph.Mutable, l *sync.RWMutex) {
	for {
		time.Sleep(time.Millisecond * time.Duration(500 + rand.Intn(1000)))

		l.Lock()
		fmt.Printf(ConsoleColorGreen("%-20s Locked\n"), "[ModifyNodes]:")

		if rand.Intn(2) == 0 && graphSize != maxGraphSize {
			graphSize++
			fmt.Printf(ConsoleColorGreen("%-20s Add node. Node count = %d\n"), "[ModifyNodes]:", graphSize)
		} else if graphSize > 1 {
			if g.Order() < 1 {
				fmt.Printf(ConsoleColorGreen("%-20s Unlocked\n"), "[ModifyNodes]:")
				l.Unlock()
				continue
			}

			graphSize--
			var from = rand.Intn(graphSize)

			fmt.Printf(ConsoleColorGreen("%-20s Delete node %d. Node count = %d\n"), "[ModifyNodes]:", from, graphSize)

			g.Visit(from, func(to int, cost int64) (skip bool) {
				g.DeleteBoth(from, to)
				return
			})
		}

		fmt.Printf(ConsoleColorGreen("%-20s Unlocked\n"), "[ModifyNodes]:")
		l.Unlock()
	}
}

func findPath(g *graph.Mutable, lock *sync.RWMutex, paint ConsoleColorFunc) {
	for {
		time.Sleep(time.Millisecond * time.Duration(500 + rand.Intn(1000)))
		lock.RLock()
		fmt.Printf(paint("%-20s Locked\n"), "[FindPath]:")

		if g.Order() < 2 {
			fmt.Printf(paint("%-20s Unlocked\n"), "[FindPath]:")
			lock.RUnlock()
			continue
		}

		from, to := getRandNodes(g.Order())
		time.Sleep(500 * time.Millisecond)

		_, cost := graph.ShortestPath(g, from, to)

		fmt.Printf(paint("%-20s Cost from %d to %d: %d\n"), "[FindPath]:", from, to, cost)
		fmt.Printf(paint("%-20s Unlocked\n"), "[FindPath]:")
		lock.RUnlock()
	}
}

func main() {
	g := graph.New(maxGraphSize)
	l := &sync.RWMutex{}

	go modifyNodes(g, l)
	go modifyEdges(g, l)
	go findPath(g, l, ConsoleColorPurple)
	go findPath(g, l, ConsoleColorYellow)

	select {}
}
