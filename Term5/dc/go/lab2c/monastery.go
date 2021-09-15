package main

type Monastery uint8

const (
	GuanYin Monastery = iota
	GuanYan
)

func (monastery Monastery) String() string {
	switch monastery {
	case GuanYin: return "GuanYin"
	case GuanYan: return "GuanYan"
	}

	return "Unknown"
}