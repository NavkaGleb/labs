package main

import (
	"fmt"
	"strings"
)

type CompetitionNode struct {
	Left 	*CompetitionNode
	Right 	*CompetitionNode
	Key 	*Monk
}

func (node *CompetitionNode) Print(level int) {
	if node.Left != nil {
		node.Left.Print(level - 1)
	}

	fmt.Printf("%v%v \n", strings.Repeat("\t\t\t", level), node.Key)

	if node.Right != nil {
		node.Right.Print(level - 1)
	}
}
