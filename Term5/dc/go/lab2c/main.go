package main

import (
	"fmt"
	"math"
	"math/rand"
)

func generateMonks(monkCount int) MonkContainer {
	monks := make(MonkContainer, monkCount)

	for i := range monks {
		monks[i] = &Monk{rand.Intn(100), Monastery(rand.Intn(2))}
	}

	return monks
}

func start(monks MonkContainer) *CompetitionNode {
	channel := make(chan *CompetitionNode)
	go round(monks, channel)

	return <-channel
}

func round(monks MonkContainer, channel chan *CompetitionNode) {
	if len(monks) == 1 {
		channel <- &CompetitionNode{nil, nil, monks[0]}
		return
	}

	leftChannel, rightChannel := make(chan *CompetitionNode), make(chan *CompetitionNode)

	go round(monks[:len(monks)/2 ], leftChannel)
	go round(monks[ len(monks)/2:], rightChannel)

	monkLeftNode, monkRightNode := <-leftChannel, <-rightChannel

	if monkLeftNode.Key.Chi > monkRightNode.Key.Chi {
		channel <- &CompetitionNode{monkLeftNode, monkRightNode, monkLeftNode.Key}
	} else {
		channel <- &CompetitionNode{monkLeftNode, monkRightNode, monkRightNode.Key}
	}

	close(channel)
}

func main() {
	const MonkCount = 16

	monks := generateMonks(MonkCount)
	root := start(monks)

	root.Print(int(math.Log2(MonkCount)))

	fmt.Printf("\nThe winner: %s!\n", root.Key.Monastery)
}
