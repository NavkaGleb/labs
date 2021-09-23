package main

import "fmt"

var (
	TerminalBlack   = Color("\033[1;30m%s\033[0m")
	TerminalRed     = Color("\033[1;31m%s\033[0m")
	TerminalGreen   = Color("\033[1;32m%s\033[0m")
	TerminalYellow  = Color("\033[1;33m%s\033[0m")
	TerminalPurple  = Color("\033[1;34m%s\033[0m")
	TerminalMagenta = Color("\033[1;35m%s\033[0m")
	TerminalTeal    = Color("\033[1;36m%s\033[0m")
	TerminalWhite   = Color("\033[1;37m%s\033[0m")
)

func Color(colorString string) func(...interface{}) string {
	sprint := func(args ...interface{}) string {
		return fmt.Sprintf(colorString, fmt.Sprint(args...))
	}

	return sprint
}
