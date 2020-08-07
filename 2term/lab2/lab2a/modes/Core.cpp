#include "Core.h"

// private methods

void Core::pushMode(const short& mode) {

	if (mode == 0)
		this->modes.push(new InteractiveMode);
	else if (mode == 1)
		this->modes.push(new DemoMode);
	else if (mode == 2)
		this->modes.push(new Benchmark(512, 3));

	// run top mode
	this->modes.top()->run();

	// clear top mode
	delete this->modes.top();
	this->modes.pop();

}

// public methods

void Core::run() {

	char response = 'y';
	short mode;

	while (response == 'y') {

		std::cout << "select the mode" << std::endl;
		std::cout << "0 - interactive mode" << std::endl
				  << "1 - demo mode" << std::endl
				  << "2 - benchmark" << std::endl;

		std::cin >> mode;
		std::cout << std::endl;

		while (mode > 2) {

			std::cout << "pfff, select existing mode!" << std::endl;
			std::cin >> mode;

		}

		this->pushMode(mode);

		std::cout << "do you want to select another mode. [y/n]" << std::endl;
		std::cin >> response;

		while (response != 'y' && response != 'n') {

			std::cout << "press 'y' or 'n'!" << std::endl;
			std::cin >> response;

		}

	}

}