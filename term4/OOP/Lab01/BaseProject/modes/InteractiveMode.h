#ifndef INTERACTIVEMODE_H
#define INTERACTIVEMODE_H

#include "DemoMode.h"

class InteractiveMode {
private:
	// variables
	DataBase& database;

public:
	// constructor / destructor
	explicit InteractiveMode(DataBase& database);
	~InteractiveMode();

	// methods
	void run();

};

#endif // INTERACTIVEMODE_H
