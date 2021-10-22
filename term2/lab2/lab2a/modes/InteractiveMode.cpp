#include "InteractiveMode.h"

// private methods

void InteractiveMode::init(const char* path, std::vector<std::string>& storage) {

	std::ifstream infile(path);

	if (!infile.is_open()) {

		std::cout << "failed to open the " << path << " file" << std::endl;
		return;

	}

	std::string currentStr;

	while(std::getline(infile, currentStr))
		storage.emplace_back(currentStr);

}

void InteractiveMode::print(const std::vector<std::string>& data) const {

	for (auto& str : data)
		std::cout << str << std::endl;

}

// constructor / destructor

InteractiveMode::InteractiveMode() {

	this->deque = nullptr;

	this->commandsPath = "../files/commands.txt";
	this->dequesPath = "../files/deques.txt";

	this->init(this->commandsPath, this->commands);
	this->init(this->dequesPath, this->deques);

}

InteractiveMode::~InteractiveMode() {

	delete this->deque;

}

// public methods

void InteractiveMode::run() {

	char response = 'y';		// client response
	short command;				// selected command
	short sdeque;				// selected deque

	// main loop
	while (response == 'y') {

		if (!this->deque) {

			this->print(this->deques);
			std::cin >> sdeque;

			if (sdeque == 0)
				this->deque = new ngca::Deque<Circle>(10000000, 4000000);
			else if (sdeque == 1)
				this->deque = new ngvc::Deque<Circle>;
			else if (sdeque == 2)
				this->deque = new ng::Deque<Circle, ng::Array>;
			else if (sdeque == 3)
				this->deque = new ng::Deque<Circle>;
			else if (sdeque == 4)
				this->deque = new stdng::Deque<Circle>;
			else
				std::cout << "try again" << std::endl;

		} else {


			this->print(this->commands);
			std::cin >> command;

			this->parseCommands(command, this->deque);

			std::cout << "do you want to continue. 'y' - yes, 'n' - no" << std::endl;
			std::cout << "if you want to continue and choose another deque press 'c'." << std::endl;
			std::cin >> response;

			while (response != 'y' && response != 'n' && response != 'c') {

				std::cout << "press 'y', 'n' or 'c'!" << std::endl;
				std::cin >> response;

			}

			if (response == 'c') {

				delete this->deque;
				this->deque = nullptr;
				response = 'y';

			}

		}

	}

}