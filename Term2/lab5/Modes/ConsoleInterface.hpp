#pragma once

#include <map>
#include <string>
#include <sstream>
#include <vector>

#include "Mode.hpp"
#include "../Graphs/Graph.hpp"

class ConsoleInterface : public Mode {
private:
	// variables
	short _type;
	std::map<std::string, ng::Graph*> _graphs;
	std::stringstream _dcommands;

	std::vector<std::string> _gcommands;

	// private methods
	void _initGraphCommands();
	void _initDemoCommands();

	template <typename T>
	void _input(T& value, const bool& nextLine = true);
	void _input(std::string& value, const bool& nextLine = true);

	void _selectGraph();
	void _addGraph();
	void _deleteGraph();

	void _runGraph(ng::Graph* graph);

public:
	// enums
	enum TYPE { INTERACTIVE = 0, DEMO = 1 };

	// constructor / destructor
	explicit ConsoleInterface(short type);
	~ConsoleInterface() override;

	// public methods
	void run() override;

};