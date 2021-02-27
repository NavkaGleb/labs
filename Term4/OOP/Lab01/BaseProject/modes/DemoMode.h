#ifndef DEMOMODE_H
#define DEMOMODE_H

#include "Benchmark.h"

class DemoMode {
private:
	// variables
	DataBase& database;
	unsigned int sleepTime;

	// methods
	void sleep();

public:
	// constructor / destructor
	explicit DemoMode(DataBase& database, unsigned int sleepTime);
	~DemoMode();

	// methods
	void run();
};

#endif // DEMOMODE_H
