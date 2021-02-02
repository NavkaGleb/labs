#ifndef DEMOMODE_H
#define DEMOMODE_H

#include "Benchmark.h"

class DemoMode : public Mode {
private:
	// variables
	std::string commandsPath;
	std::string demoCommandsPath;

	ng::Deque<Circle> deque;

	// methods
	void sleep() const;

public:
	// constructor / destructor
	DemoMode();

	// methods
	void run() override;

};

#endif // DEMOMODE_H
