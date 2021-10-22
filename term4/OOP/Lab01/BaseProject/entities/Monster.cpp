#include "Monster.h"

// init static variables

unsigned int Monster::globalId = 0;

// constructor

Monster::Monster(const std::string& name, const unsigned short hp, const unsigned short damage, const float specialAttack,
	const std::string& specialAttackType, const DateTime datetime, const unsigned int locationId, const bool infile) {

	//  init variables
	this->id = Monster::globalId;
	this->name = name;
	this->hp = hp;
	this->damage = damage;
	this->specialAttack = specialAttack;
	this->specialAttackType = specialAttackType;
	this->datetime = datetime;
	this->locationId = locationId;
	this->infile = infile;

	if (!infile)
		Monster::globalId++;

}

// accessors

const unsigned int& Monster::getId() {

	return this->id;

}

const std::string& Monster::getName() {

	return this->name;

}

const unsigned int& Monster::getLocationId() {

	return this->locationId;

}

const bool& Monster::getInfile() {

	return this->infile;

}

// modifiers

void Monster::setId(const unsigned _id) {

	this->id = _id;

}

void Monster::setLocationId(const unsigned int _locationId) {

	this->locationId = _locationId;

}

void Monster::setInfile(const bool _infile) {

	this->infile = _infile;

}

// methods

void Monster::initGlobalId(const std::string& path) {

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

	infile >> Monster::globalId;
	Monster::globalId++;

	infile.close();

}

bool Monster::print(const std::string& path) {

	std::ifstream infile(path);

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
	unsigned int id = 0;
	std::string name;
	unsigned short hp = 0;
	unsigned short damage = 0;
	float specialAttack = 0.f;
	std::string specialAttackType;
	DateTime datetime;
	unsigned int locationId = 0;

	std::cout << "id\tname\thp\tdamage\ttime\t\tdate\t\tlocId\tspAtt\t\tspAttT\n";

	while (infile >> id >> name >> hp >> damage >> specialAttack >> specialAttackType >>
		datetime.year >> datetime.month >> datetime.day >>
		datetime.hours >> datetime.minutes >> datetime.seconds >> locationId) {

		std::cout << id << '\t' << name << '\t' << hp << '\t' << damage << '\t' << datetime.toFormatString() << '\t' <<
			locationId << '\t' << std::fixed << std::setprecision(5) << specialAttack << '\t' << '\t' <<
			specialAttackType << '\n';

	}

	infile.close();

	return true;

}

bool Monster::bprint(const std::string& path) {

	std::ifstream infile(path, std::ios_base::binary);

	if (!infile.is_open()) {

		std::cout << "failed to open the " << path << " file" << '\n';
		return false;

	}

	// check for empty file
	infile.seekg(0, std::ios_base::end);
	if (infile.tellg() == 0) {

		std::cout << "the file is empty" << '\n';
		return false;

	}

	// monster variables
	unsigned int id;
	unsigned short nameLength;
	std::string name;
	unsigned short hp;
	unsigned short damage;
	float specialAttack;
	unsigned short specialAttackTypeLength;
	std::string specialAttackType;
	DateTime dt;
	unsigned int locationId;

	// file variables
	char ch;
	unsigned int fileEnd = 0;

	// init fileEnd
	fileEnd = infile.tellg();
	infile.seekg(0, std::ios_base::beg);

	std::cout << "id\tname\thp\tdamage\ttime\t\tdate\t\tlocId\tspAtt\t\tspAttT\n";

	// main file loop
	while (infile.tellg() != fileEnd) {

		infile.read((char*)&id, sizeof(id));
		infile.read((char*)&nameLength, sizeof(nameLength));

		for (unsigned short i = 0; i < nameLength; i++)
			infile.get(ch), name += ch;

		infile.read((char*)&hp, sizeof(hp));
		infile.read((char*)&damage, sizeof(damage));
		infile.read((char*)&specialAttack, sizeof(specialAttack));
		infile.read((char*)&specialAttackTypeLength, sizeof(specialAttackTypeLength));

		for (unsigned short i = 0; i < specialAttackTypeLength; i++)
			infile.get(ch), specialAttackType += ch;

		infile.read((char*)&dt.year, sizeof(dt.year));
		infile.read((char*)&dt.month, sizeof(dt.month));
		infile.read((char*)&dt.day, sizeof(dt.day));
		infile.read((char*)&dt.hours, sizeof(dt.hours));
		infile.read((char*)&dt.minutes, sizeof(dt.minutes));
		infile.read((char*)&dt.seconds, sizeof(dt.seconds));
		infile.read((char*)&locationId, sizeof(locationId));

		std::cout << id << '\t' << name << '\t' << hp << '\t' << damage << '\t' << dt.toFormatString() << '\t'<<
			locationId << '\t' << std::fixed << std::setprecision(5) << specialAttack << '\t' << '\t' <<
			specialAttackType << '\n';

		name.clear();
		specialAttackType.clear();

	}


	infile.close();

	return true;

}

void Monster::saveToFile(std::ofstream& outfile, bool& emptyFile, const bool printResponse) {

	if (emptyFile)
		emptyFile = false;
	else
		outfile << '\n';

	outfile << this->id << " " <<  this->name << " " << this->hp << " " << this->damage << " " <<
		this->specialAttack << " " << this->specialAttackType << " " << datetime.toString() << " " <<
		this->locationId;

	if (printResponse)
		std::cout << this->name << " successfully saved to the file" << '\n';

}

void Monster::saveToBinFile(std::ofstream& outfile, const bool printResponse) {

	unsigned short nameLength = this->name.length();
	unsigned short specialAttackTypeLength = this->specialAttackType.length();

	outfile.write((char*)&this->id, sizeof(this->id));
	outfile.write((char*)&nameLength, sizeof(nameLength));
	outfile << this->name;
	outfile.write((char*)&this->hp, sizeof(this->hp));
	outfile.write((char*)&this->damage, sizeof(this->damage));
	outfile.write((char*)&this->specialAttack, sizeof(this->specialAttack));
	outfile.write((char*)&specialAttackTypeLength, sizeof(specialAttackTypeLength));
	outfile << this->specialAttackType;
	outfile.write((char*)&this->datetime.year, sizeof(this->datetime.year));
	outfile.write((char*)&this->datetime.month, sizeof(this->datetime.month));
	outfile.write((char*)&this->datetime.day, sizeof(this->datetime.day));
	outfile.write((char*)&this->datetime.hours, sizeof(this->datetime.hours));
	outfile.write((char*)&this->datetime.minutes, sizeof(this->datetime.minutes));
	outfile.write((char*)&this->datetime.seconds, sizeof(this->datetime.seconds));
	outfile.write((char*)&this->locationId, sizeof(this->locationId));

	this->infile = true;

	if (printResponse)
		std::cout << this->name << " successfully saved to the binary file" << '\n';

}

std::ostream& operator <<(std::ostream& out, Monster& monster) {

	out << monster.id << '\t' << monster.name << '\t' << monster.hp << '\t' << monster.damage << '\t' <<
		  monster.datetime.toFormatString() << '\t' << monster.locationId << '\t' <<
		  std::fixed << std::setprecision(5) << monster.specialAttack  << "\t\t" << monster.specialAttackType;

	return out;

}