#pragma once

#include <stack>

#include "Mode.hpp"

class Core {
private:
	// variables
	std::stack<Mode*> _modes;

public:
	// constructor / destructor
	Core() = default;
	virtual ~Core() = default;

	// public methods
	void run();

};