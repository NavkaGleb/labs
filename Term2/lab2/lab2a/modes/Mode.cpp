#include "Mode.h"

// private methods

void Mode::parseCommands(const short& command, ng::DequeInterface<Circle>* deque, const bool showCommand) const {

	static std::mt19937 mt(std::clock());
	static std::uniform_real_distribution<double> drand(-10000, 10000);

	if (showCommand)
		std::cout << "command: " << command << std::endl;

	if (command == 0) {

		std::cout << "size = " << deque->size() << std::endl;

	} else if (command == 1) {

		std::cout << "front element: ";
		std::cout << deque->front() << std::endl;

	} else if (command == 2) {

		std::cout << "back element: ";
		std::cout << deque->back() << std::endl;

	} else if (command == 3) {

		Circle circle(drand(mt), drand(mt), drand(mt));
		std::cout << "circle: " << circle << std::endl;
		deque->pushFront(circle);
		std::cout << "successfully was pushed to front of the deque" << std::endl;

	} else if (command == 4) {

		Circle circle(drand(mt), drand(mt), drand(mt));
		std::cout << "circle: " << circle << std::endl;
		deque->pushBack(circle);
		std::cout << "successfully was pushed to back of the deque" << std::endl;

	} else if (command == 5) {

		std::cout << deque->front() << std::endl;
		deque->popFront();
		std::cout << "the front element of deque successfully was deleted" << std::endl;

	} else if (command == 6) {

		std::cout << deque->back() << std::endl;
		deque->popBack();
		std::cout << "the back element of deque successfully was deleted" << std::endl;

	} else if (command == 7) {

		if (deque->empty())
			std::cout << "deque is empty" << std::endl;
		else
			std::cout << "deque is not empty" << std::endl;

	} else if (command == 8) {

		deque->clear();
		std::cout << "deque successfully was cleared" << std::endl;

	} else {

		std::cout << "pfff, choose something adequate" << std::endl;

	}

	std::cout << std::endl;

}