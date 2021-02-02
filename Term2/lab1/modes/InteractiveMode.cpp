#include "InteractiveMode.h"

// constructor / destructor

InteractiveMode::InteractiveMode(DataBase& database) : database(database) {}

InteractiveMode::~InteractiveMode() {

	// delete all dynamic memory
	DataBase::deleteLocationsFromMemory(this->database.getLocations());
	this->database.deleteMonstersFromMemory(false);

}

// methods

void InteractiveMode::run() {

	char response = 'y';
	short command;

	while (response == 'y') {

		std::cout << '\n';
		DataBase::print(this->database.getCommands1());

		std::cin >> command;
		std::cout << '\n';

		// check command
		if (command == 0)
			this->database.printFileLocations();
		else if (command == 1)
			this->database.printMemoryLocations();
		else if (command == 2)
			this->database.createLocation();
		else if (command == 3)
			this->database.createLocations(true);
		else if (command == 4)
			this->database.loadLocations(true);
		else if (command == 5)
			this->database.saveLocations(this->database.getLocations(), true);
		else if (command == 6)
			this->database.deleteLocations(true, true);
		else if (command == 7)
			DataBase::deleteLocationsFromMemory(this->database.getLocations(), true);
		else if (command == 8)
			this->database.printFileMonsters();
		else if (command == 9)
			this->database.printBinMonsters();
		else if (command == 10)
			this->database.printMemoryMonsters();
		else if (command == 11)
			this->database.createMonster();
		else if (command == 12)
			this->database.createMonsters(true);
		else if (command == 13)
			this->database.loadMonsters(true);
		else if (command == 14)
			this->database.loadBinMonsters(true);
		else if (command == 15)
			this->database.saveMonsters(true);
		else if (command == 16)
			this->database.deleteMonsterFromFile();
		else if (command == 17)
			this->database.deleteMonsterFromMemory(true);
		else if (command == 18)
			this->database.deleteMonsters(true, true, true);
		else if (command == 19)
			this->database.deleteMonstersFromMemory(true);
		else if (command == 20)
			this->database.updateMonster();
		else if (command == 21)
			this->database.search();
		else
			std::cout << "you enter unreal command (((" << '\n';

		// response input
		std::cout << "\ndo you want to continue? press 'y' or 'n'" << '\n';
		std::cin >> response;

		// check response
		while (response != 'y' && response != 'n') {

			std::cout << "please, press 'y' or 'n'!" << '\n';
			std::cin >> response;

		}

	}

}