package main

import "fmt"

var (
	ConsoleColorBlack   = consoleColor("\033[1;30m%s\033[0m")
	ConsoleColorRed     = consoleColor("\033[1;31m%s\033[0m")
	ConsoleColorGreen   = consoleColor("\033[1;32m%s\033[0m")
	ConsoleColorYellow  = consoleColor("\033[1;33m%s\033[0m")
	ConsoleColorPurple  = consoleColor("\033[1;34m%s\033[0m")
	ConsoleColorMagenta = consoleColor("\033[1;35m%s\033[0m")
	ConsoleColorTeal    = consoleColor("\033[1;36m%s\033[0m")
	ConsoleColorWhite   = consoleColor("\033[1;37m%s\033[0m")
)

type ConsoleColorFunc func(...interface{}) string

func consoleColor(colorString string) ConsoleColorFunc {
	sprint := func(args ...interface{}) string {
		return fmt.Sprintf(colorString, fmt.Sprint(args...))
	}

	return sprint
}
