#include "Core.hpp"

#include <iostream>

// methods
void Core::run() {

	short mode;

	while (true) {

		std::cout << "select the mode" << std::endl;
		std::cout << "0 - interactive mode" << "\n"
				  << "1 - demo mode" << "\n"
				  << "2 - exit" << std::endl;
		std::cout << "mode:";
		std::cin >> mode;
		std::cout << std::endl;

		if (mode == 0)
			this->_modes.push(new Mode(MODE_TYPE::INTERACTIVE));
		else if (mode == 1)
			this->_modes.push(new Mode(MODE_TYPE::DEMO));
		else if (mode == 2)
			break;
		else
			std::cout << "you entered a nonexistent command ((( try again" << std::endl;

		this->_modes.top()->run();
		delete this->_modes.top();
		this->_modes.pop();

	}

}