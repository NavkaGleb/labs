#ifndef INTERACTIVEMODE_H
#define INTERACTIVEMODE_H

#include "DemoMode.h"

class InteractiveMode : public Mode {
private:
	// variables
	ng::DequeInterface<Circle>* deque;

	const char* commandsPath;
	const char* dequesPath;

	std::vector<std::string> commands;
	std::vector<std::string> deques;

	// methods
	static void init(const char* path, std::vector<std::string>& storage);
	void print(const std::vector<std::string>& data) const;

public:
	// constructor / destructor
	InteractiveMode();
	~InteractiveMode() override;

	// methods
	void run() override;

};

#endif // INTERACTIVEMODE_H