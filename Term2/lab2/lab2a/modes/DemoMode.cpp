#include "DemoMode.h"

// private methods

void DemoMode::sleep() const {

	clock_t endTime = std::clock() + 3000 * CLOCKS_PER_SEC / 1000;

	while (std::clock() < endTime) {}

}

// constructor / destructor

DemoMode::DemoMode() {

	this->commandsPath = "../files/commands.txt";
	this->demoCommandsPath = "../files/demoCommands.txt";

}

// public methods

void DemoMode::run() {

	char response;
	short command;
	std::string str;
	std::stringstream ss;

	std::ifstream infile(this->commandsPath);

	// print commands
	while (std::getline(infile, str))
		std::cout << str << std::endl;

	str.clear();
	infile.close();

	std::cout << "do you want to load sequence of commands. [y/n]" << std::endl;
	std::cin >> response;

	if (response == 'y') {

		std::ifstream filein(this->demoCommandsPath);

		std::getline(filein, str);
		ss.str(str);

		std::cout << "sequence of commands from file: " << str << std::endl << std::endl;
		this->sleep();

		filein.close();

	} else {

		std::cout << "enter sequence of commands" << std::endl;
		std::cin.ignore();
		std::getline(std::cin, str);

		ss.str(str);

	}

	while (ss >> command) {

		this->parseCommands(command, &this->deque, true);
		this->sleep();

	}

}