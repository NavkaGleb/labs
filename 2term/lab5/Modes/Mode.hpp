#pragma once

class Mode {
public:
	// constructor / destructor
	Mode() = default;
	virtual ~Mode() = default;

	// public methods
	virtual void run() = 0;

}; // class Mode