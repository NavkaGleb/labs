#include "Core.hpp"

// public methods

void Core::run() {

	char response = 'y';
	int mode;

	while (response == 'y') {

		std::cout << "choose the mode" << std::endl;
		std::cout << "0 - demo mode" << std::endl << "1 - benchmark" << std::endl;

		std::cin >> mode;

		if (mode == 0)
			this->modes.push(new DemoMode(1800, 660, 400));
		else if (mode == 1)
			this->modes.push(new Benchmark(12));

		this->modes.top()->run();
		delete this->modes.top();
		this->modes.pop();

		std::cout << "do you want to continue? [y/n]" << std::endl;
		std::cin >> response;

		while (response != 'y' && response != 'n') {

			std::cout << "please enter [y/n]" << std::endl;
			std::cin >> response;

		}

	}

}