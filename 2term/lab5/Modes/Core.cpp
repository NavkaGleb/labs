#include "Core.hpp"

#include <iostream>

#include "ConsoleInterface.hpp"
#include "Benchmark.hpp"

// public methods
void Core::run() {

	short mode;

	while (true) {

		std::cout << "select the mode" << "\n"
		          << "0 - interactive mode" << "\n"
		          << "1 - demo mode" << "\n"
		          << "2 - benchmark" << "\n"
		          << "3 - exit" << std::endl;
		std::cout << "mode:";
		std::cin >> mode;
		std::cout << std::endl;

		if (mode == 0)
			this->_modes.emplace(new ConsoleInterface(ConsoleInterface::TYPE::INTERACTIVE));
		else if (mode == 1)
			this->_modes.emplace(new ConsoleInterface(ConsoleInterface::TYPE::DEMO));
		else if (mode == 2)
			this->_modes.emplace(new Benchmark);
		else if (mode == 3)
			break;
		else
			std::cout << "you enter not existing mode. try again" << std::endl;

		if (!this->_modes.empty())
			this->_modes.top()->run();

	}

}