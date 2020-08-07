#pragma once

#include <stack>

#include "DemoMode.hpp"
#include "Benchmark.hpp"

class Core {
private:
	// variables
	std::stack<Mode*> modes;

public:
	// constructor / destructor
	Core() = default;
	virtual ~Core() = default;

	// methods
	void run();

};