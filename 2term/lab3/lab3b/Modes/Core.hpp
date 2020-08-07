#ifndef CORE_HPP
#define CORE_HPP

#include <stack>

#include "Mode.hpp"

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

#endif // CORE_HPP
