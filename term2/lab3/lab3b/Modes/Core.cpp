#include "Core.hpp"

#include <iostream>

#include "InteractiveMode.hpp"
#include "Benchmark.hpp"

// methods

void Core::run() {

	char response = 'y';
	int mode;

	while (response == 'y') {

		std::cout << "choose the mode" << std::endl;
		std::cout << "0 - interactive mode" << std::endl << "1 - benchmark" << std::endl;

		std::cin >> mode;

		if (mode == 0)
			this->modes.push(new InteractiveMode);
		else if (mode == 1)
			this->modes.push(new Benchmark(13));

		this->modes.top()->run();
		delete this->modes.top();
		this->modes.pop();

		std::cout << "do you want to select another mode? [y/n]" << std::endl;
		std::cin >> response;

		while (response != 'y' && response != 'n') {

			std::cout << "please enter [y/n]" << std::endl;
			std::cin >> response;

		}

	}

}