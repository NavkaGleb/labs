#include "DemoMode.h"

// private methods

void DemoMode::sleep() {

	clock_t endTime = clock() + this->sleepTime * CLOCKS_PER_SEC / 1000;
	while (clock() < endTime) {}

}

// constructor

DemoMode::DemoMode(DataBase& database, unsigned int sleepTime)
	: database(database), sleepTime(sleepTime) {}

DemoMode::~DemoMode() {

	// delete all dynamic memory
	DataBase::deleteLocationsFromMemory(this->database.getLocations());
	this->database.deleteMonstersFromMemory();

}

// public methods

void DemoMode::run() {

	std::string inputCommands;
	std::stringstream ss;
	short command = 0;

	DataBase::print(this->database.getCommands2());

	// input commands
	std::cout << '\n' << "select the commands and write them with a space" << '\n';
	std::cin.ignore();
	std::getline(std::cin, inputCommands);

	// init ss
	ss.str(inputCommands);

	// parse commands
	while (ss >> command) {

		std::cout << '\n' << "current command is: " << command << '\n';

		if (command == 0) {

			/* print all locations from file */
			this->database.printFileLocations();

		} else if (command == 1) {

			/* print all locations from memory */
			this->database.printMemoryLocations();

		} else if (command == 2) {

			/* create new location */

			// locations variables
			std::string name;
			double area;
			unsigned int monstersMax;
			ss >> name >> area >> monstersMax;
			this->database.createLocation(name, area, monstersMax);

		} else if (command == 3) {

			/* create 'n' random locations */

			unsigned amount;
			ss >> amount;
			this->database.createLocations(true, amount);

		} else if (command == 4) {

			/* load all locations from file to memory */
			this->database.loadLocations(true);

		} else if (command == 5) {

			/* save all locations from memory to file */
			this->database.saveLocations(this->database.getLocations(), true);

		} else if (command == 6) {

			/* delete all locations from file */
			this->database.deleteLocations(true, true);

		} else if (command == 7) {

			/* delete all locations from memory */
			DataBase::deleteLocationsFromMemory(this->database.getLocations(), true);

		} else if (command == 8) {

			/* print all monsters from file */
			this->database.printFileMonsters();

		} else if (command == 9) {

			/* print all monsters from binary file */
			this->database.printBinMonsters();

		} else if (command == 10) {

			/* print all monsters from memory */
			this->database.printMemoryMonsters();

		} else if (command == 11) {

			/* create new monster */

			// monster variables
			std::string name;
			unsigned short hp;
			unsigned short damage;
			float specialAttack;
			short selectedAttackType;
			std::string specialAttackType;
			unsigned int locationId;

			ss >> name >> hp >> damage >> specialAttack >> selectedAttackType >> locationId;

			this->database.createMonster(name, hp, damage, specialAttack, selectedAttackType, locationId);

		} else if (command == 12) {

			/* create 'n' random monsters */

			unsigned int amount;
			ss >> amount;
			this->database.createMonsters(true, amount);

		} else if (command == 13) {

			/* load all monsters from file to memory */
			this->database.loadMonsters(true);

		} else if (command == 14) {

			/* load all monsters from binary file to memory */
			this->database.loadBinMonsters(true);

		} else if (command == 15) {

			/* save monsters from memory to files */
			this->database.saveMonsters(true);

		} else if (command == 16) {

			/* delete monster from file */

			unsigned int id;
			ss >> id;
			this->database.deleteMonsterFromFile(id);

		} else if (command == 17) {

			/* delete monster from memory */

			int index;
			ss >> index;
			this->database.deleteMonsterFromMemory(index);

		} else if (command == 18) {

			/* delete all monsters from files */
			this->database.deleteMonsters(true, true, true);

		} else if (command == 19) {

			/* delete all monsters from memory */
			this->database.deleteMonstersFromMemory(true);

		} else if (command == 20) {

			/* update monster */

			std::map<std::string, std::string> values;

			ss >> values["id"] >> values["name"] >> values["hp"] >> values["damage"] >> values["specialAttack"] >>
			   values["specialAttackType"] >> values["time"] >> values["date"] >> values["locationId"];

			this->database.updateMonster(values);

		} else if (command == 21) {

			/* search by name fragment */

			std::string nameFragment;
			ss >> nameFragment;
			this->database.search(nameFragment);

		} else if (command == 22) {

			/* search by hp and by damage */

			unsigned short hp;
			unsigned short damage;
			ss >> hp >> damage;
			this->database.search(hp, damage);

		} else if (command == 23) {

			/* search by attack type and by time / date */

			short selectedAttack;
			std::string time;
			std::string date;
			DateTime datetime;
			ss >> selectedAttack >> time >> date;
			datetime.setTime(time);
			datetime.setDate(date);
			this->database.search(selectedAttack, datetime);

		} else if (command == 24) {

			/* search by area range, by attack type and by specialAttack */
			double leftArea;
			double rightArea;
			unsigned short attackType;
			float specialAttack;
			ss >> leftArea >> rightArea >> attackType >> specialAttack;
			this->database.search(leftArea, rightArea, attackType, specialAttack);

		}

		this->sleep();

	}

}