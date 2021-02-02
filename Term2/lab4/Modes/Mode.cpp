#include "Mode.hpp"

#include <fstream>
#include <functional>
#include <chrono>
#include <thread>

// private methods
void Mode::_sleep(const int& delayTime) {

	std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));

}

std::string Mode::_initCommands() {

	std::string commands;

	std::ifstream infile("../Files/DemoModeCommands.txt");

	if (!infile.is_open())
		throw std::invalid_argument("failed to open the file | Mode");

	std::getline(infile, commands);
	infile.close();

	return commands;

}

void Mode::_initCommands(const std::string& treeType, const char* filepath) {

	std::ifstream infile(filepath);

	if (!infile.is_open())
		throw std::invalid_argument("failed to open the file | Mode");

	std::string currentCommand;

	while (std::getline(infile, currentCommand))
		this->_commands[treeType].emplace_back(currentCommand);

	infile.close();

}

void Mode::_print(const std::string& treeType) const {

	for (const auto& command : this->_commands.at(treeType))
		std::cout << command << std::endl;

}

void Mode::_printTrees() const {

	for (const auto& p : this->_trees)
		std::cout << p.first << " - " << p.second->type() << std::endl;

}

template <typename T>
void Mode::_input(T& value) {
	
	if (this->_type == MODE_TYPE::INTERACTIVE) {
		
		std::cin >> value;

	} else if (this->_type == MODE_TYPE::DEMO) {

		this->_dcommands >> value;

		std::cout << " " << value << std::endl;
		std::cout << "..." << std::endl;
		Mode::_sleep(3000);

	}

}

void Mode::_input(std::string& value) {

	if (this->_type == MODE_TYPE::INTERACTIVE) {

		std::cin >> value;

	} else if (this->_type == MODE_TYPE::DEMO) {

		this->_dcommands >> value;

		std::cout << " ";
		for (const auto& ch : value)
			this->_sleep(300), std::cout << ch;
		std::cout << "\n..." << std::endl;
		Mode::_sleep(3000);

	}

}

void Mode::_input(std::list<int>& path) {

	int cindex;

	if (this->_type == MODE_TYPE::INTERACTIVE) {

		std::string indexes;
		std::stringstream ss;

		std::cin.ignore();

		std::getline(std::cin, indexes);
		ss.str(indexes);

		while (ss >> cindex) path.emplace_back(cindex);

	} else if (this->_type == MODE_TYPE::DEMO) {

		int range;
		this->_dcommands >> range;

		std::cout << " ";

		for (int i = 0; i < range; i++) {

			this->_dcommands >> cindex;
			path.push_back(cindex);
			this->_sleep(300);
			std::cout << cindex << " ";

		}

		std::cout << std::endl;
		std::cout << "..." << std::endl;

		this->_sleep(3000);

	}

}

void Mode::_input(short& command, const std::string& treeType) {

	std::cout << "select the command" << std::endl;
	this->_print(treeType);
	std::cout << "command:";
	
	if (this->_type == MODE_TYPE::INTERACTIVE) {
		
		std::cin >> command;
		
	} else if (this->_type == MODE_TYPE::DEMO) {
		
		this->_dcommands >> command;
		std::cout << " " << command << std::endl;
		std::cout << "..." << std::endl;
		Mode::_sleep(3000);
		
	}

}

void Mode::_addTree() {

	short type;
	std::string name;
	ng::Tree* newTree = nullptr;

	std::cout << "enter name of the new Tree" << std::endl;
	std::cout << "name:";
	this->_input(name);

	while (!newTree) {

		std::cout << "select type of the new Tree" << "\n"
				  << "0 - GeneralTree (int value)" << "\n"
				  << "1 - BinarySearchTree (int value)" << "\n"
				  << "2 - ExpressionTree" << std::endl;
		std::cout << "type:";
		this->_input(type);

		if (type == 0)
			newTree = new ng::GeneralTree<int>;
		else if (type == 1)
			newTree = new ng::BinarySearchTree<int>;
		else if (type == 2)
			newTree = new ng::ExpressionTree;
		else
			std::cout << "you entered a nonexistent type ((( try again" << std::endl;

	}

	this->_trees[name] = newTree;

	std::cout << "new tree " << name << " was successfully added" << std::endl;
	std::cout << std::endl;

}

ng::Tree* Mode::_selectTree() {

	std::string name;

	if (this->_trees.empty()) {

		std::cout << "there is no Tree, so you need to add new one" << std::endl << std::endl;
		return nullptr;

	}

	while (name.empty()) {

		std::cout << "select one of existing trees" << std::endl;
		this->_printTrees();
		std::cout << "name:";

		this->_input(name);

		std::cout << std::endl;

	}

	return this->_trees[name];

}

void Mode::_deleteTree() {

	if (this->_trees.empty()) {

		std::cout << "no trees (((" << std::endl;

	} else {

		std::string name;

		std::cout << "select one of existing trees" << std::endl;
		this->_printTrees();
		std::cout << "name:";
		this->_input(name);

		delete this->_trees[name];
		this->_trees.erase(name);

		std::cout << "tree " << name << " was successfully deleted" << std::endl;

	}

	std::cout << std::endl;

}

void Mode::_addTree(ng::Tree* tree) {

	char response;

	std::cout << "do you want to add this tree to memory?" << std::endl;
	std::cout << "[y/n]:";
	this->_input(response);

	while (response != 'y' && response != 'n') {

		std::cout << "enter 'y' or 'n'" << std::endl;
		this->_input(response);

	}

	if (response == 'y') {

		std::string name;

		std::cout << "enter name of the tree" << std::endl;
		std::cout << "name:";
		this->_input(name);

		this->_trees[name] = tree;

		std::cout << "new tree " << name << " was successfully added to the memory" << std::endl;

	} else {

		delete tree;
		std::cout << "tree was successfully deleted" << std::endl;

	}

}

void Mode::_generalTreeRun(ng::Tree* parent) {

	short command;
	auto* tree = dynamic_cast<ng::GeneralTree<int>*>(parent);

	while (true) {

		this->_input(command, tree->type());

		if (command == 0) {

			if (!tree->empty()) {

				std::list<int> path;

				std::cout << "enter path (example: 0 1 2 ...):";
				this->_input(path);

				auto* node = tree->path(path);

				if (!node) std::cout << "node not found" << std::endl;
				else std::cout << "found node: " << *node << std::endl;

			} else {

				std::cout << "tree is empty" << std::endl;

			}

		} else if (command == 1) {

			int value;
			std::list<int> path;

			std::cout << "enter value (int):";
			std::cin >> value;

			std::cout << "enter path to parent (example: 0 1 2 ...):";
			this->_input(path);

			std::cout << "current path = ";
			for (const auto& e : path)
				std::cout << e << " ";
			std::cout << std::endl;

			auto* node = tree->add(value, path);

			if (!node) std::cout << "you enter nonexistent path to parent" << std::endl;
			else std::cout << "new node: " << *node << std::endl;

		} else if (command == 2) {

			int value;
			float probability;

			std::cout << "enter value (int):";
			this->_input(value);

			std::cout << "enter probability (float [0, 1]):";
			this->_input(probability);

			auto* node = tree->add(value, probability);

			if (!node) std::cout << "node wasn't added" << std::endl;
			else std::cout << "new node was successfully added. new node: " << *node << std::endl;

		} else if (command == 3) {

			int amount;

			std::cout << "enter amount:";
			this->_input(amount);

			for (int i = 0; i < amount; i++)
				tree->add(ng::rand::irand(-20, 20), ng::rand::frand(0.f, 1.f));

			std::cout << "success!" << std::endl;

		} else if (command == 4) {

			if (!tree->empty())
				tree->tprint();
			else
				std::cout << "tree is empty" << std::endl;

		} else if (command == 5) {

			if (!tree->empty())
				tree->pprint();
			else
				std::cout << "tree is empty" << std::endl;

		} else if (command == 6) {

			if (!tree->empty()) {

				std::list<int> path;

				tree->pprint();

				std::cout << "enter path to node (example: 0 1 2 ...):";
				this->_input(path);

				ng::GeneralTree<int>* deletedTree = tree->eraseSubTree(path);

				if (!deletedTree) {

					std::cout << "you enter nonexistent path to node" << std::endl;

				} else {

					std::cout << "deleted tree = ";
					deletedTree->pprint();

					this->_addTree(deletedTree);

				}

			} else {

				std::cout << "tree is empty" << std::endl;

			}

		} else if (command == 7) {

			if (!tree->empty()) {

				int amount;
				int value;

				std::cout << "enter value (int):";
				this->_input(value);

				amount = tree->eraseSubTree(value);

				std::cout << "amount of deleted nodes = " << amount << std::endl;

			} else {

				std::cout << "tree is empty" << std::endl;

			}

		} else if (command == 8) {

			std::cout << std::endl;
			break;

		} else {

			std::cout << "you entered a nonexistent command ((( try again" << std::endl;

		}

		if (this->_type == MODE_TYPE::DEMO)
			std::cout << "next command in 5 seconds ..." << std::endl, this->_sleep(5000);

		std::cout << std::endl;

	}

}

void Mode::_binaryTreeRun(ng::Tree* parent) {

	short command;
	auto* tree = dynamic_cast<ng::BinarySearchTree<int>*>(parent);

	while (true) {

		this->_input(command, tree->type());

		if (command == 0) {

			int value;

			std::cout << "enter value (int):";
			this->_input(value);

			tree->add(value);

			std::cout << "'" << value << "' was successfully added to the tree" << std::endl;

		} else if (command == 1) {

			int amount;

			std::cout << "enter amount (int):";
			this->_input(amount);

			for (int i = 0; i < amount; i++)
				tree->add(ng::rand::irand(-100, 100));

			std::cout << "new random '" << amount << "' was successfully added to the tree" << std::endl;

		} else if (command == 2) {

			if (!tree->empty()) {

				tree->toThreaded();
				std::cout << "transition from BinarySearchTree to ThreadBinaryTree was successful" << std::endl;

			} else {

				std::cout << "tree is empty" << std::endl;

			}

		} else if (command == 3) {

			if (!tree->empty()) {

				tree->removeThreads();
				std::cout << "transition from ThreadBinaryTree to BinarySearchTree was successful" << std::endl;

			} else {

				std::cout << "three is empty" << std::endl;

			}

		} else if (command == 4) {

			if (!tree->empty())
				tree->tprint();
			else
				std::cout << "tree is empty";

		} else if (command == 5) {

			if (!tree->empty())
				tree->oprint(" ");
			else
				std::cout << "tree is empty" << std::endl;

		} else if (command == 6) {

			std::cout << std::endl;
			break;

		} else {

			std::cout << "you entered a nonexistent command ((( try again" << std::endl;

		}

		if (this->_type == MODE_TYPE::DEMO)
			std::cout << "next command in 5 seconds ..." << std::endl, this->_sleep(5000);

		std::cout << std::endl;

	}

}

void Mode::_expressionTreeRun(ng::Tree* parent) {

	short command;
	auto* tree = dynamic_cast<ng::ExpressionTree*>(parent);

	while (true) {

		this->_input(command, tree->type());

		if (command == 0) {

			std::string expression;

			std::cout << "enter math expression:";
			this->_input(expression);

			tree->build(expression);

			std::cout << "tree was successfully build" << std::endl;

		} else if (command == 1) {

			if (!tree->empty()) {

				std::cout << "expression: ";
				tree->bypass();

			} else {

				std::cout << "tree is empty" << std::endl;

			}

		} else if (command == 2) {

			if (!tree->empty()) {

				if (this->_type == MODE_TYPE::INTERACTIVE) {

					std::cout << tree->calc() << std::endl;

				} else if (this->_type == MODE_TYPE::DEMO) {

					std::map<std::string, double> variables;
					std::string key;
					double value;
					int range;

					this->_dcommands >> range;

					std::cout << "variables" << std::endl;

					for (int i = 0; i < range; i++) {

						this->_dcommands >> key;
						this->_dcommands >> value;

						std::cout << key << ": " << value << std::endl;

					}

					std::cout << "result: " << tree->calc(variables) << std::endl;

				}

			} else {

				std::cout << "tree is empty" << std::endl;

			}

		} else if (command == 3) {

			if (!tree->empty())
				std::cout << tree->result() << std::endl;
			else
				std::cout << "tree is empty" << std::endl;

		} else if (command == 4) {

			if (!tree->empty()) {

				tree->simplify();
				std::cout << "tree was successfully simplified" << std::endl;

			} else {

				std::cout << "tree is empty" << std::endl;

			}

		} else if (command == 5) {

			if (!tree->empty()) {

				ng::ExpressionTree oldTree(*tree);

				if (this->_type == MODE_TYPE::INTERACTIVE) {

					if (oldTree == tree->differentiate())
						std::cout << "failed in differentiation" << std::endl;
					else
						std::cout << "tree was successfully differentiated" << std::endl;

				} else if (this->_type == MODE_TYPE::DEMO) {

					std::string variable;

					std::cout << "there are all variables in expression: ";
					tree->vprint();
					std::cout << "choose a variable by which you will differentiate:";
					this->_dcommands >> variable;
					std::cout << " " << variable << std::endl;

					if (oldTree == tree->differentiate(variable))
						std::cout << "failed in differentiation" << std::endl;
					else
						std::cout << "tree was successfully differentiated" << std::endl;

				}

			} else {

				std::cout << "tree is empty" << std::endl;

			}

		} else if (command == 6) {

			if (!tree->empty()) {

				ng::ExpressionTree* newTree = tree->ndifferentiate();

				if (!newTree) {

					std::cout << "failed in differentiation" << std::endl;

				} else {

					std::cout << "new tree = ";
					newTree->print();
					std::cout << "expression = ";
					newTree->bypass();

					this->_addTree(newTree);

				}

			} else {

				std::cout << "tree is empty" << std::endl;

			}

		} else if (command == 7) {

			if (!tree->empty())
				tree->print();
			else
				std::cout << "tree is empty" << std::endl;

		} else if (command == 8) {

			std::cout << std::endl;
			break;

		} else {

			std::cout << "you entered a nonexistent command ((( try again" << std::endl;

		}

		if (this->_type == MODE_TYPE::DEMO)
			std::cout << "next command in 5 seconds ..." << std::endl, this->_sleep(5000);

		std::cout << std::endl;

	}

}

void Mode::_runTree(ng::Tree* parent) {

	if (!parent)
		return;

	if (parent->type() == "ng::GeneralTree")
		this->_generalTreeRun(parent);
	else if (parent->type() == "ng::BinarySearchTree")
		this->_binaryTreeRun(parent);
	else if (parent->type() == "ng::ExpressionTree")
		this->_expressionTreeRun(parent);
	else
		std::cout << "fuck this shit" << std::endl;

}

void Mode::_run() {

	short command;
	bool stop = false;

	while (!stop) {

		std::cout << "select the command" << "\n"
				  << "0 - add new Tree" << "\n"
				  << "1 - delete existing Tree" << "\n"
				  << "2 - select existing Tree" << "\n"
				  << "3 - select another mode" << std::endl;
		std::cout << "command:";
		this->_input(command);
		std::cout << std::endl;

		if (command == 0)
			this->_addTree();
		else if (command == 1)
			this->_deleteTree();
		else if (command == 2)
			this->_runTree(this->_selectTree());
		else if (command == 3)
			stop = true;
		else
			std::cout << "you entered a nonexistent command ((( try again" << std::endl;

	}

}

// constructor / destructor
Mode::Mode(short type) : _type(type) {

	this->_initCommands("ng::GeneralTree", "../Files/GeneralTreeCommands.txt");
	this->_initCommands("ng::BinarySearchTree", "../Files/BinarySearchTreeCommands.txt");
	this->_initCommands("ng::ExpressionTree", "../Files/ExpressionTreeCommands.txt");

}

Mode::~Mode() {

	for (auto& p : this->_trees)
		delete p.second;

}

// public methods
void Mode::run() {

	if (this->_type == MODE_TYPE::DEMO)
		this->_dcommands.str(this->_initCommands());

	this->_run();

}