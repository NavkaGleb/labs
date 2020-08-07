#include "InteractiveMode.hpp"

#include <iostream>
#include <fstream>

#include "../algo/algo.hpp"

// private methods

void InteractiveMode::initCommands() {

	std::ifstream infile("../Files/interactiveModeCommands.txt");

	if (!infile.is_open())
		throw std::invalid_argument("failed to open the file | initCommands | InteractiveMode");

	for (std::string str; std::getline(infile, str); )
		this->commands.emplace_back(str);

	infile.close();

}

void InteractiveMode::initCfunctions() {

	// name
	this->cfunctions[0] = [](const Monster& a, const Monster& b) {
		return (a).name() < (b).name() ? 1 : (a).name() > (b).name() ? -1 : 0;
	};
	this->cfunctions[1] = [](const Monster& a, const Monster& b) {
		return (a).name() < (b).name() ? -1 : (a).name() > (b).name() ? 1 : 0;
	};

	// health
	this->cfunctions[2] = [](const Monster& a, const Monster& b) {
		return (a).hp() < (b).hp() ? 1 : (a).hp() > (b).hp() ? -1 : 0;
	};
	this->cfunctions[3] = [](const Monster& a, const Monster& b) {
		return (a).hp() < (b).hp() ? -1 : (a).hp() > (b).hp() ? 1 : 0;
	};

	// damage
	this->cfunctions[4] = [](const Monster& a, const Monster& b) {
		return (a).damage() < (b).damage() ? 1 : (a).damage() > (b).damage() ? -1 : 0;
	};
	this->cfunctions[5] = [](const Monster& a, const Monster& b) {
		return (a).damage() < (b).damage() ? -1 : (a).damage() > (b).damage() ? 1 : 0;
	};

	// attack chance
	this->cfunctions[6] = [](const Monster& a, const Monster& b) {
		return (a).achance() < (b).achance() ? 1 : (a).achance() > (b).achance() ? -1 : 0;
	};
	this->cfunctions[7] = [](const Monster& a, const Monster& b) {
		return (a).achance() < (b).achance() ? -1 : (a).achance() > (b).achance() ? 1 : 0;
	};

	// attack type
	this->cfunctions[8] = [](const Monster& a, const Monster& b) {
		return (a).atype() < (b).atype() ? 1 : (a).atype() > (b).atype() ? -1 : 0;
	};
	this->cfunctions[9] = [](const Monster& a, const Monster& b) {
		return (a).atype() < (b).atype() ? -1 : (a).atype() > (b).atype() ? 1 : 0;
	};

}

void InteractiveMode::print() {

	for (const auto& command : this->commands)
		std::cout << command << std::endl;

}

void InteractiveMode::mprint() {

	if (this->monsters.empty()) {

		std::cout << "no monsters (" << std::endl;
		return;

	}

	for (const auto& monster : this->monsters)
		std::cout << monster << std::endl;

}

void InteractiveMode::mgenerate() {

	static int amount;

	std::cout << "enter amount" << std::endl;
	std::cin >> amount;

	for (int i = 0; i < amount; i++)
		this->monsters.emplace_back(Monster());

}

void InteractiveMode::sort() {

	static int key;

	std::cout << "choose the field" << std::endl;
	std::cout << "0 - name (ascending)\t\t" << "1 - name (descending)" << std::endl;
	std::cout << "2 - health (ascending)\t\t" << "3 - health (descending)" << std::endl;
	std::cout << "4 - damage (ascending)\t\t" << "5 - damage (descending)" << std::endl;
	std::cout << "6 - attack chance (ascending)\t" << "7 - attack chance (descending)" << std::endl;
	std::cout << "8 - attack type (ascending)\t" << "9 - attack type (descending)" << std::endl;
	std::cin >> key;

	ng::algo::quicksort(this->monsters.begin(), this->monsters.end() - 1, this->cfunctions[key]);

}

// constructor / destructor

InteractiveMode::InteractiveMode() {

	this->initCommands();
	this->initCfunctions();

}

// public methods

void InteractiveMode::run() {

	char response = 'y';
	int command;

	while (response == 'y') {

		std::cout << "choose the command" << std::endl;
		this->print();

		std::cin >> command;

		if (command == 0)
			this->mprint();
		else if (command == 1)
			this->mgenerate();
		else if (command == 2)
			this->sort();
		else if (command == 3)
			response = 'n';
		else
			std::cout << "ehh. please, choose existing command" << std::endl;

		std::cout << std::endl;

	}

}