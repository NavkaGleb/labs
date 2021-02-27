#include "Location.h"

// init static variables

unsigned int Location::globalId = 0;

// constructor

Location::Location(const std::string& name, double area, unsigned int monstersMax, bool saved) {

	this->id = Location::globalId;
	this->name = name;
	this->area = area;
	this->monstersMax = monstersMax;
	this->currentMonsters = 0;

	if (!saved)
		Location::globalId++;

}

// accessors

const unsigned int& Location::getId() {

	return this->id;

}

const std::string& Location::getName() {

	return this->name;

}

const unsigned int& Location::getMonstersMax() {

	return this->monstersMax;

}

const unsigned int& Location::getCurrentMonsters() {

	return this->currentMonsters;

}

// modifiers

void Location::setId(const unsigned int _id) {

	this->id = _id;

}

void Location::setCurrentMonsters(const unsigned int _currentMonsters) {

	this->currentMonsters = _currentMonsters;

}

// methods

void Location::initGlobalId(const std::string& path) {

	std::ifstream infile(path);

	if (!infile.is_open()) {

		std::cout << "failed to open the " << path << " file" << '\n';
		return;

	}

	bool keepLooping = true;

	infile.seekg(-1, std::ios_base::end);

	while (keepLooping) {

		char ch = 0;
		infile.get(ch);

		if(infile.tellg() <= 1)
			infile.seekg(0), keepLooping = false;
		else if(ch == '\n')
			keepLooping = false;
		else
			infile.seekg(-2, std::ios_base::cur);

	}

	infile >> Location::globalId;
	Location::globalId++;

	infile.close();

}

bool Location::print(const std::string& path) {

	std::ifstream infile(path, std::ios_base::app);

	if (!infile.is_open()) {

		std::cout << "failed to open the " << path << "file" << '\n';
		return false;

	}

	// check for empty file
	infile.seekg(0, std::ios_base::end);
	if (infile.tellg() == 0) {

		std::cout << "the file is empty" << '\n';
		return false;

	}

	infile.seekg(0, std::ios_base::beg);

	// iteration variables
	unsigned int id;
	std::string name;
	double area;
	unsigned int monstersMax;
	unsigned int currentMonsters;

	std::cout << "id\tname\t\tarea\t\tmons max\tcurrent monsters\n";

	while (infile >> id >> name >> area >> monstersMax >> currentMonsters)
		std::cout << id << '\t' << name << '\t' << std::fixed << std::setprecision(4) << area << '\t' <<
		monstersMax << '\t' << '\t' << currentMonsters << '\n';

	infile.close();

	return true;

}

void Location::incrementMonsters() {

	this->currentMonsters++;

}

void Location::decrementMonsters() {

	this->currentMonsters--;

}

void Location::save(std::ofstream& outfile, bool& emptyFile) {

	if (emptyFile)
		emptyFile = false;
	else
		outfile << '\n';

	outfile << this->id << ' ' << this->name << ' ' << this->area << ' ' << this->monstersMax << ' ' << this->currentMonsters;

}

std::ostream& operator <<(std::ostream& out, const Location& location) {

	out << location.id << '\t' <<  location.name << '\t' << std::fixed << std::setprecision(4) << location.area <<
		'\t' << location.monstersMax << '\t' << '\t' << location.currentMonsters;

	return out;

}