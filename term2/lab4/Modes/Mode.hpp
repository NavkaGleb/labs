#pragma once

#include <map>
#include <sstream>
#include <list>

#include "../Trees/GeneralTree.hpp"
#include "../Trees/BinarySearchTree.hpp"
#include "../Trees/ExpressionTree.hpp"

// enums
enum MODE_TYPE { UNKNOWN = -1, INTERACTIVE, DEMO };

class Mode {
private:
	// variables
	short _type;
	std::map<std::string, ng::Tree*> _trees;
	std::map<std::string, std::vector<std::string>> _commands;
	std::stringstream _dcommands;

	// private methods
	static void _sleep(const int& delayTime);
	static std::string _initCommands();
	void _initCommands(const std::string& treeType, const char* filepath);
	void _print(const std::string& treeType) const;
	void _printTrees() const;

	template <typename T>
	void _input(T& value);

	void _input(std::string& value);
	void _input(std::list<int>& path);
	void _input(short& command, const std::string& treeType);

	void _addTree();
	void _deleteTree();
	ng::Tree* _selectTree();

	void _addTree(ng::Tree* tree);

	void _generalTreeRun(ng::Tree* parent);
	void _binaryTreeRun(ng::Tree* parent);
	void _expressionTreeRun(ng::Tree* parent);
	void _runTree(ng::Tree* parent);
	void _run();

public:
	// constructor / destructor
	explicit Mode(short type);
	virtual ~Mode();

	// public methods
	void run();

};