#ifndef CORE_H
#define CORE_H

#include "InteractiveMode.h"

class Core {
private:
	// variables
	ng::Stack<Mode*> modes;

	// methods
	void pushMode(const short& mode);

public:
	// constructor
	Core() = default;

	// methods
	void run();

};

#endif // CORE_H
