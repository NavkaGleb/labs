#ifndef INTERACTIVEMODE_HPP
#define INTERACTIVEMODE_HPP

#include <vector>
#include <string>
#include <map>
#include <functional>

#include "Mode.hpp"
#include "../Entities/Monster.hpp"

class InteractiveMode : public Mode {
private:
	// variables
	std::vector<std::string> commands;
	std::vector<Monster> monsters;
	std::map<int, std::function<int(const Monster&, const Monster&)>> cfunctions;			// compare function

	// methods
	void initCommands();
	void initCfunctions();

	void print();
	void mprint();																			// monster print
	void mgenerate();																		// monster generate
	void sort();

public:
	// constructor / destructor
	InteractiveMode();
	~InteractiveMode() override = default;

	// methods
	void run() override;

};

#endif // INTERACTIVEMODE_HPP
