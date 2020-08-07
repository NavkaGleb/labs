#include "DataBase.h"

// private methods

void DataBase::initMenu(const std::string& path, std::vector<std::string>& menu) {

	/* load menu from the file to the vector */

	std::ifstream infile(path);

	if (!infile.is_open()) {

		std::cout << "failed to open the " << path << " file" << '\n';
		return;

	}

	std::string currentLine;

	while (std::getline(infile, currentLine))
		menu.emplace_back(currentLine);

	infile.close();

}

unsigned int DataBase::getId(const std::string& str) {

	/* get monsterId from the string */

	unsigned int result = 0;

	for (unsigned int i = 0; i < str.length() && str[i] != ' '; i++)
		result = result * 10 + int(str[i] - 48);

	return result;

}

unsigned int DataBase::getLocationId(const std::string& str) {

	/* get locationId from the string */

	unsigned int locationId = 0;
	unsigned int dozen = 1;

	for (unsigned int i = str.length() - 1; i >= 0; i--) {

		if (str[i] == ' ')
			break;

		locationId = int(str[i] - 48) * dozen + locationId;
		dozen *= 10;

	}

	return locationId;

}

bool DataBase::emptyFile(const std::string& path) {

	/* return true if file is empty, otherwise return false */

	std::ifstream infile(path);

	infile.seekg(0, std::ios_base::end);
	if (infile.tellg() == 0)
		return true;

	infile.close();

	return false;

}

template <typename T>
int DataBase::getIndex(std::vector<T>& data, const unsigned int id) {

	/* return the position the element is at */

	int left = 0;
	int right = data.size() - 1;
	int middle = 0;

	while (left <= right) {

		middle = (left + right) / 2;

		if (data[middle]->getId() == id)
			return middle;
		else if (data[middle]->getId() > id)
			right = middle - 1;
		else if (data[middle]->getId() < id)
			left = middle + 1;

	}

	return -1;

}

template <typename T>
int DataBase::getPosition(std::vector<T>& data, const unsigned int id){

	/* return -1 if item exist in vector, otherwise return the position at which it should be */

	// -1 exist !

	int left = 0;
	int right = static_cast<int>(data.size()) - 1;
	int middle = 0;

	while (left <= right) {

		middle = (left + right) / 2;

		if (data[middle]->getId() == id)
			return -1;
		else if (data[middle]->getId() > id)
			right = middle - 1;
		else if (data[middle]->getId() < id)
			left = middle + 1;

	}

	if (middle + 1 == data.size())
		return middle + 1;
	return middle;

}

void DataBase::saveToBinFile(const std::vector<std::string>& data) {

	/* save data from vector to the binary file */

	unsigned int idM;
	std::string name;
	unsigned short hp;
	unsigned short damage;
	float specialAttack;
	std::string attackType;
	DateTime dt;
	unsigned int locationId;

	unsigned short nameLength;
	unsigned short attackLength;

	std::ofstream outfile(this->mbpath, std::ios_base::binary);
	std::stringstream ss;

	for (auto& line : data) {

		// init stringstream
		ss.str(line);

		// init monster variables
		ss >> idM >> name >> hp >> damage >> specialAttack >> attackType >>
		   dt.year >> dt.month >> dt.day >> dt.hours >> dt.minutes >> dt.seconds >> locationId;

		// init strings length
		nameLength = name.length();
		attackLength = attackType.length();

		// write to the binary file
		outfile.write((char*)&idM, sizeof(idM));
		outfile.write((char*)&nameLength, sizeof(nameLength));
		outfile << name;
		outfile.write((char*)&hp, sizeof(hp));
		outfile.write((char*)&damage, sizeof(damage));
		outfile.write((char*)&specialAttack, sizeof(specialAttack));
		outfile.write((char*)&attackLength, sizeof(attackLength));
		outfile << attackType;
		outfile.write((char*)&dt.year, sizeof(dt.year));
		outfile.write((char*)&dt.month, sizeof(dt.month));
		outfile.write((char*)&dt.day, sizeof(dt.day));
		outfile.write((char*)&dt.hours, sizeof(dt.hours));
		outfile.write((char*)&dt.minutes, sizeof(dt.minutes));
		outfile.write((char*)&dt.seconds, sizeof(dt.seconds));
		outfile.write((char*)&locationId, sizeof(locationId));

		// clear strings
		name.clear();
		attackType.clear();

	}

	outfile.close();

}

unsigned int DataBase::stoui(const std::string& str) {

	/* string to unsigned int */

	unsigned int result = 0;

	for (char i : str)
		result = result * 10 + int(i - 48);

	return result;

}

unsigned short DataBase::stous(const std::string& str) {

	/* string to unsigned short */

	unsigned short result = 0;

	for (char i : str)
		result = result * 10 + short(i - 48);

	return result;

}

// constructor

DataBase::DataBase(const std::string& mpath, const std::string& mbpath, const std::string& lPath) {

	// init Monster and Location globalIds
	Location::initGlobalId(lPath);
	Monster::initGlobalId(mpath);

	this->mpath = mpath;
	this->mbpath = mbpath;
	this->lpath = lPath;

	// some hard code
	this->modesPath = "../files/modes.txt";
	this->commands1Path = "../files/commands1.txt";
	this->commands2Path = "../files/commands2.txt";

	DataBase::initMenu(this->modesPath, this->modes);
	DataBase::initMenu(this->commands1Path, this->commands1);
	DataBase::initMenu(this->commands2Path, this->commands2);

	// init random
	this->mt = std::mt19937(this->rd());
	this->ushort_rand = std::uniform_int_distribution<unsigned short>(2, USHRT_MAX);
	this->float_rand = std::uniform_real_distribution<float>(0, 1);
	this->double_rand = std::uniform_real_distribution<double>(100, 2000);

}

// accessors

std::vector<std::string>& DataBase::getModes() {

	return this->modes;

}

std::vector<std::string>& DataBase::getCommands1() {

	return this->commands1;

}

std::vector<std::string>& DataBase::getCommands2() {

	return this->commands2;

}

std::vector<Location*>& DataBase::getLocations() {

	return this->locations;

}

// public methods

void DataBase::print(const std::vector<std::string>& commands) {

	/* print command from the vector */

	for (auto& command : commands)
		std::cout << command << '\n';

}

// ------------------------- Locations -------------------------

bool DataBase::printFileLocations() {

	/* print all location that are saved in the file */

	return Location::print(this->lpath);

}

bool DataBase::printMemoryLocations() {

	/* print locations that are saved in the memory */

	if (this->locations.empty()) {

		std::cout << "no locations in memory(" << '\n';
		return false;

	}

	std::cout << "id\tname\t\tarea\t\tmons max\tcurrent monsters" << '\n';
	for (auto& location : this->locations)
		std::cout << *location << '\n';

	return true;

}

void DataBase::createLocation() {

	/* create new location and push it to the memory */

	// location variables
	std::string name;
	double area;
	unsigned int monstersMax;

	// input
	std::cout << "enter name ( string )" << '\n';
	std::cin >> name;

	std::cout << "enter area ( double )" << '\n';
	std::cin >> area;

	std::cout << "enter monstersMax ( unsigned int )" << '\n';
	std::cin >> monstersMax;

	// push new location to the memory
	this->locations.emplace_back(new Location(name, area, monstersMax));

	std::cout << "successfully add new location to memory\n";

}

void DataBase::createLocation(const std::string& name, const double area, const unsigned int monstersMax) {

	/* create new location and push it to the memory */

	this->locations.emplace_back(new Location(name, area, monstersMax));

	std::cout << "successfully add new location to memory\n";

}

void DataBase::createLocations(const bool showResponse, unsigned int amount) {

	/* create new random locations and push them to the memory */

	// limit variables
	const unsigned short wordLength = 9;

	// location variables
	std::string name;
	double area;
	unsigned int monstersMax;

	if (amount == 0) {

		std::cout << "enter amount of locations" << '\n';
		std::cin >> amount;

	}

	for (unsigned int i = 0; i < amount; i++) {

		for (unsigned int j = 0; j < this->ushort_rand(this->mt) % wordLength + 8; j++)
			name += SYMBOLS[this->ushort_rand(this->mt) % SYMBOL_AMOUNT];

		area = this->double_rand(this->mt);
		monstersMax = this->ushort_rand(this->mt) % 30000;

		// push new location to the memory
		this->locations.emplace_back(new Location(name, area, monstersMax));

		// clear strings
		name.clear();

	}

	if (showResponse)
		std::cout << "successfully created " << amount << " new locations and saved in memory" << '\n';

}

void DataBase::loadLocations(const bool showResponse) {

	/* load locations from the file to the locations memory to the correct position in memory */

	// location variables
	unsigned int id = 0;
	std::string name;
	double area = 0.f;
	unsigned int monstersMax = 0;
	unsigned int currentMonsters = 0;

	int position = 0;

	std::ifstream infile(this->lpath);

	// main input loop
	while (infile >> id >> name >> area >> monstersMax >> currentMonsters) {

		// init position in memory
		position = DataBase::getPosition(this->locations, id);
		if (position != -1) {

			this->locations.insert(
				this->locations.begin() + position,
				new Location(name, area, monstersMax, true)
			);
			this->locations[position]->setId(id);
			this->locations[position]->setCurrentMonsters(currentMonsters);

		}

	}

	if (showResponse)
		std::cout << "all locations from file were successfully load to memory" << '\n';

}

bool DataBase::loadLocations(std::vector<Location*>& storage, const bool showResponse) {

	/* load locations from the file to the storage */

	// locations variables
	unsigned int id;
	std::string name;
	double area;
	unsigned int monstersMax;
	unsigned int currentMonsters;

	std::ifstream infile(this->lpath);

	while (infile >> id >> name >> area >> monstersMax >> currentMonsters) {

		storage.emplace_back(new Location(name, area, monstersMax, true));
		storage.back()->setId(id);
		storage.back()->setCurrentMonsters(currentMonsters);

	}

	if (showResponse)
		std::cout << "all locations from file were successfully load to memory" << '\n';

	return !storage.empty();

}

void DataBase::saveLocations(std::vector<Location*>& storage, const bool showResponse) {

	/* save locations from storage to the file. delete locations from the storage */

	std::ofstream outfile(this->lpath, std::ios_base::app);

	if (!outfile.is_open()) {

		std::cout << "failed to open the " << this->lpath << " file" << '\n';
		return;

	}

	if (storage.empty()) {

		if (showResponse)
			std::cout << "no locations in memory (((" << '\n';
		return;

	}

	bool emptyFile = this->emptyFile(this->lpath);

	// save locations to the file and delete them from the storage
	for (auto& location : storage) {

		location->save(outfile, emptyFile);
		delete location;

	}

	storage.clear();

	outfile.close();

	if (showResponse)
		std::cout << "all locations were successfully saved to " << this->lpath << " file" << '\n';

}

void DataBase::deleteLocations(const bool deleteMonsters, const bool showResponse) {

	/* delete monsters from files, memory and locations from file */

	// delete monsters
	if (deleteMonsters) {

		this->deleteMonstersFromMemory(true);
		this->deleteMonsters(true, true, true);

	}

	std::ofstream outfile(this->lpath, std::ios_base::trunc);
	outfile.close();

	if (showResponse)
		std::cout << "all locations were successfully deleted from " << this->lpath << " file" << '\n';

}

void DataBase::deleteLocationsFromMemory(std::vector<Location*>& storage, const bool showResponse) {

	/* delete locations from pass memory */

	if (storage.empty()) {

		if (showResponse)
			std::cout << "no locations (((" << '\n';
		return;

	}

	// delete all locations
	for (auto& location : storage)
		delete location;
	storage.clear();

	if (showResponse)
		std::cout << "all locations were successfully deleted from memory" << '\n';

}

// ------------------------- Monsters -------------------------

bool DataBase::printFileMonsters() {

	/* print all monsters that are saved in the file */

	return Monster::print(this->mpath);

}

bool DataBase::printBinMonsters() {

	/* print all monsters that are saved in the binary file */

	return Monster::bprint(this->mbpath);

}

bool DataBase::printMemoryMonsters() {

	/* print all monsters that are saved in the memory */

	if (monsters.empty()) {

		std::cout << "memory is empty(" << '\n';
		return false;

	}

	std::cout << "idV\tidM\tname\thp\tdamage\ttime\t\tdate\t\tlocId\tspAttack\tspAttackType\n";
	for (unsigned int i = 0; i < monsters.size(); i++)
		std::cout << i << '\t' << *monsters[i] << '\n';

	return true;

}

void DataBase::createMonster() {

	/* create new monster and push it to the memory */

	// monster variables
	std::string name;
	unsigned short hp = 0;
	unsigned short damage = 0;
	float specialAttack = 0.f;
	short selectedAttackType = 0;
	std::string specialAttackType;
	DateTime datetime;
	unsigned int locationId = 0;

	// input
	std::cout << "enter name ( string )" << '\n';
	std::cin.ignore(1, '\n');
	std::getline(std::cin, name);

	std::cout << "enter hp ( unsigned short )" << '\n';
	std::cin >> hp;

	std::cout << "enter damage ( unsigned short )" << '\n';
	std::cin >> damage;

	std::cout << "enter special attack ( float > 0 && < 1 )" << '\n';
	std::cin >> specialAttack;

	std::cout << "select special attack type\n";
	for (unsigned int i = 0; i < AMOUNT_ATTACKS; i++)
		std::cout << i << " - " << ATTACK_TYPES[i] << '\n';
	std::cin >> selectedAttackType;
	specialAttackType = ATTACK_TYPES[selectedAttackType];

	datetime.setCurrentTime();

	std::cout << "select the location id ( unsigned int )" << '\n';
	if (this->printFileLocations())
		std::cin >> locationId;

	// push to the memory
	this->monsters.emplace_back(new Monster(name, hp, damage, specialAttack, specialAttackType, datetime, locationId));

	std::cout << "successfully add new monster to memory\n";

}

void DataBase::createMonster(const std::string& name, const unsigned short hp, const unsigned short damage,
	const float specialAttack, const short selectedAttackType, unsigned int locationId) {

	/* create new monster and push it to the memory */

	// init monster variables
	std::string specialAttackType = ATTACK_TYPES[selectedAttackType];
	DateTime datetime;
	datetime.setCurrentTime();

	// push to the memory
	this->monsters.emplace_back(new Monster(name, hp, damage, specialAttack, specialAttackType, datetime, locationId));

	std::cout << "successfully add new monster to memory\n";

}

bool DataBase::createMonsters(const bool showResponse, unsigned int amount) {

	/* create random monsters and push the to the memory */

	// limit variables
	const unsigned short wordLength = 6;

	// monster variables
	std::string name;
	unsigned short hp;
	unsigned short damage;
	float specialAttack;
	std::string specialAttackType;
	DateTime dt;
	unsigned int locationId = 0;

	// locations variables
	unsigned int locationIndex;
	Location* currentLocation;
	std::vector<Location*> lstorage;
	std::vector<Location*> fullLstorage;

	// load locations
	this->loadLocations(lstorage);

	// check for no locations
	if (lstorage.empty()) {

		if (showResponse)
			std::cout << "no locations in file. you need to create them" << '\n';
		return false;

	}

	// input amount
	if (amount == 0) {

		std::cout << "enter amount of monsters" << '\n';
		std::cin >> amount;

	}

	// main loop which generate monsters
	for (unsigned int i = 0; i < amount; i++) {

		// init monster variables
		for (unsigned int j = 0; j < this->ushort_rand(this->mt) % wordLength + 2; j++)
			name += SYMBOLS[this->ushort_rand(this->mt) % SYMBOL_AMOUNT];

		hp = this->ushort_rand(this->mt) % 50000;
		damage = this->ushort_rand(this->mt) % 1000;
		specialAttack = this->float_rand(this->mt);
		specialAttackType = ATTACK_TYPES[this->ushort_rand(this->mt) % AMOUNT_ATTACKS];
		dt.setRandTime(this->mt, this->ushort_rand);

		while (!lstorage.empty()) {

			locationIndex = this->ushort_rand(this->mt) % lstorage.size();
			currentLocation = lstorage[locationIndex];

			locationId = currentLocation->getId();

			if (currentLocation->getCurrentMonsters() < currentLocation->getMonstersMax()) {

				currentLocation->incrementMonsters();
				break;

			} else {

				fullLstorage.emplace_back(currentLocation);
				lstorage.erase(lstorage.begin() + locationIndex);

			}

		}

		if (lstorage.empty()) {

			if (showResponse)
				std::cout << "all locations are full (((" << '\n';
			return false;

		}

		// push new monster to memory
		this->monsters.emplace_back(new Monster(name, hp, damage, specialAttack, specialAttackType, dt, locationId));

		// clear strings
		name.clear();
		specialAttackType.clear();

	}

	int position;
	for (auto& flocation : fullLstorage) {

		position = DataBase::getPosition(lstorage, flocation->getId());
		lstorage.insert(lstorage.begin() + position, flocation);

	}

	// clear all dynamic memory and update locations in the file
	this->deleteLocations(false, false);
	this->saveLocations(lstorage);

	if (showResponse)
		std::cout << "successfully created " << amount << " new monsters and saved to the memory" << '\n';

	return true;

}

unsigned long long DataBase::loadMonsters(const bool showResponse) {

	/* load monsters from file to the memory */

	std::ifstream infile(this->mpath);

	if (!infile.is_open()) {

		std::cout << "failed to open the " << this->mpath << " file" << '\n';
		return -1;

	}

	// monster variables
	unsigned int id = 0;
	std::string name;
	unsigned short hp = 0;
	unsigned short damage = 0;
	float specialAttack = 0.f;
	std::string specialAttackType;
	DateTime dt;
	unsigned int locationId = 0;

	int position;
	unsigned long long fileSize = 0;

	// init fileSize
	infile.seekg(0, std::ios_base::end);
	fileSize = infile.tellg();
	infile.seekg(0, std::ios_base::beg);

	while (infile >> id >> name >> hp >> damage >> specialAttack >> specialAttackType >>
		dt.year >> dt.month >> dt.day >> dt.hours >> dt.minutes >> dt.seconds >> locationId) {

		// init position
		position = DataBase::getPosition(this->monsters, id);

		if (position != -1) {

			this->monsters.insert(
				this->monsters.begin() + position,
				new Monster(name, hp, damage, specialAttack, specialAttackType, dt, locationId, true)
			);
			this->monsters[position]->setId(id);

		}

	}

	infile.close();

	if (showResponse)
		std::cout << "all monsters from file were successfully load to the memory" << '\n';

	return fileSize;

}

unsigned long long DataBase::loadBinMonsters(const bool showResponse) {

	/* load monsters from the binary file to the memory */

	std::ifstream infile(this->mbpath, std::ios_base::binary);

	if (!infile.is_open()) {

		std::cout << "failed to open the " << this->mbpath << " file" << '\n';
		return -1;

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

	char ch;
	int position;
	unsigned long long fileSize = 0;

	// init fileSize
	infile.seekg(0, std::ios_base::end);
	fileSize = infile.tellg();
	infile.seekg(0, std::ios_base::beg);

	// main file loop
	while (infile.tellg() != fileSize) {

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

		position = DataBase::getPosition(this->monsters, id);

		if (position != -1) {

			this->monsters.insert(
				this->monsters.begin() + position,
				new Monster(name, hp, damage, specialAttack, specialAttackType, dt, locationId, true)
			);
			this->monsters[position]->setId(id);

		}

		// clear strings
		name.clear();
		specialAttackType.clear();

	}

	infile.close();

	if (showResponse)
		std::cout << "all monsters from binary file were successfully load to the memory" << '\n';

	return fileSize;

}

void DataBase::saveMonstersToFile(const bool showResponse) {

	/* save monsters from the memory to the file */

	std::ofstream outfile(this->mpath, std::ios_base::app);
	bool emptyFile = this->emptyFile(this->mpath);

	// check for saved
	for (auto& monster : this->monsters)
		if (!monster->getInfile())
			monster->saveToFile(outfile, emptyFile);

	outfile.close();

	if (showResponse)
		std::cout << "all monsters were successfully saved to " << this->mpath << " file" << '\n';

}

void DataBase::saveMonstersToBinFile(const bool showResponse) {

	/* save monsters from the memory to the binary file */

	std::ofstream outfile(this->mbpath, std::ios_base::binary | std::ios_base::app);

	// check for saved
	for (auto& monster : monsters)
		if (!monster->getInfile())
			monster->saveToBinFile(outfile);

	outfile.close();

	if (showResponse)
		std::cout << "all monsters were successfully saved to " << this->mbpath << " file" << '\n';

}

void DataBase::saveMonsters(const bool showResponse) {

	/* save monsters from memory to the files */

	if (this->monsters.empty()) {

		std::cout << "memory is empty(" << '\n';
		return;

	}

	this->saveMonstersToFile(showResponse);
	this->saveMonstersToBinFile(showResponse);

}

void DataBase::deleteMonsterFromFile() {

	/* delete certain monster from file, binary file and also from the memory */

	if (this->printFileMonsters()) {

		unsigned int selectedId = 0;
		unsigned int locationId = 0;
		std::string currentLine;
		std::vector<std::string> fileLines;

		std::cout << '\n' << "enter the id of the monster you want to delete" << '\n';
		std::cin >> selectedId;

		// ------------------------ memory ------------------------

		int position = DataBase::getIndex(this->monsters, selectedId);
		if (position != -1)
			this->monsters.erase(this->monsters.begin() + position);

		// ------------------------ simple file ------------------------

		// read
		std::ifstream infile(this->mpath);

		while (std::getline(infile, currentLine)) {

			if (this->getId(currentLine) != selectedId)
				fileLines.emplace_back(currentLine);
			else
				locationId = this->getLocationId(currentLine);

		}

		infile.close();

		// write
		std::ofstream outfile(this->mpath);

		for (unsigned int i = 0; i < fileLines.size() - 1; i++)
			outfile << fileLines[i] << '\n';
		outfile << fileLines.back();

		outfile.close();

		// ------------------------ binary file ------------------------

		this->saveToBinFile(fileLines);

		// ------------------------ update locations ------------------------

		// location variables
		int index;
		std::vector<Location*> lstorage;

		// init location variables
		this->loadLocations(lstorage);
		index = DataBase::getIndex(lstorage, locationId);

		lstorage[index]->decrementMonsters();

		this->deleteLocations(false, false);
		this->saveLocations(lstorage);

		std::cout << "selected monster was successfully deleted from file" << '\n';

	}

}

void DataBase::deleteMonsterFromFile(const unsigned int id) {

	/* delete certain monster from file, binary file and also from the memory */

	unsigned int locationId = 0;
	std::string currentLine;
	std::vector<std::string> fileLines;

	// ------------------------ memory ------------------------

	int position = DataBase::getIndex(this->monsters, id);
	if (position != -1)
		this->monsters.erase(this->monsters.begin() + position);

	// ------------------------ simple file ------------------------

	// read
	std::ifstream infile(this->mpath);

	while (std::getline(infile, currentLine)) {

		if (getId(currentLine) != id)
			fileLines.emplace_back(currentLine);
		else
			locationId = this->getLocationId(currentLine);

	}

	infile.close();

	// write
	std::ofstream outfile(this->mpath);

	for (unsigned int i = 0; i < fileLines.size() - 1; i++)
		outfile << fileLines[i] << '\n';
	outfile << fileLines.back();

	outfile.close();

	// ------------------------ binary file ------------------------

	this->saveToBinFile(fileLines);

	// ------------------------ update locations ------------------------

	std::vector<Location*> lstorage;

	// init location variables
	this->loadLocations(lstorage);

	lstorage[DataBase::getIndex(lstorage, locationId)]->decrementMonsters();

	// rewrite location file
	this->deleteLocations(false, false);
	this->saveLocations(lstorage);

}

void DataBase::deleteMonsterFromMemory(const bool showResponse, int index) {

	/* delete certain monster from memory and update locations in file */

	if (this->monsters.empty()) {

		std::cout << "memory is empty(" << '\n';
		return;

	}

	if (index == -1) {

		this->printMemoryMonsters();

		std::cout << '\n' << "enter the idV of the monster you want to delete" << '\n';
		std::cin >> index;

	}

	if (index < monsters.size()) {

		this->monsters.erase(this->monsters.begin() + index);

		std::vector<Location*> lstorage;

		// init lstorage
		this->loadLocations(lstorage);

		lstorage[DataBase::getIndex(lstorage, this->monsters[index]->getLocationId())]->decrementMonsters();

		// rewrite location file
		this->deleteLocations(false, false);
		this->saveLocations(lstorage);

		if (showResponse)
			std::cout << "the monster was successfully deleted" << '\n';

	}

}

void DataBase::deleteMonstersFromFile(const bool showResponse) {

	/* delete all monsters from file */

	std::ofstream outfile(this->mpath, std::ios_base::trunc);
	outfile.close();

	if (showResponse)
		std::cout << "all monsters were successfully deleted from " << this->mpath << " file" << '\n';

}

void DataBase::deleteMonstersFromBinFile(const bool showResponse) {

	/* delete all monsters from binary file */

	std::ofstream outfile(this->mbpath, std::ios_base::trunc);
	outfile.close();

	if (showResponse)
		std::cout << "all monsters were successfully deleted from " << this->mbpath << " file" << '\n';

}

void DataBase::deleteMonsters(const bool file, const bool bfile, const bool showResponse) {

	/* delete monsters from files */

	// update Monster::infile in memory
	for (unsigned int i = 0; i < this->monsters.size(); i++) {

		if (this->monsters[i]->getInfile()) {

			delete this->monsters[i];
			this->monsters.erase(this->monsters.begin() + i--);

		}

	}

	std::vector<Location*> lstorage;

	// load locations from file
	this->loadLocations(lstorage);

	// set currentMonster to 0
	for (auto& location : lstorage)
		location->setCurrentMonsters(0);

	// iterate over locations and set currentMonsters
	for (auto& monster : this->monsters)
		lstorage[DataBase::getIndex(lstorage, monster->getLocationId())]->incrementMonsters();

	// rewrite location file
	this->deleteLocations(false, false);
	this->saveLocations(lstorage);

	// delete from files
	if (file)
		this->deleteMonstersFromFile(showResponse);

	if (bfile)
		this->deleteMonstersFromBinFile(showResponse);

}

void DataBase::deleteMonstersFromMemory(const bool showResponse) {

	/* delete all monsters from the memory */

	if (this->monsters.empty()) {

		if (showResponse)
			std::cout << "no monsters in memory (((" << '\n';
		return;

	}

	for (auto& monster : this->monsters)
		delete monster;
	this->monsters.clear();

	if (showResponse)
		std::cout << "all monsters were successfully deleted from memory" << '\n';

}

void DataBase::updateMonster() {

	/* update certain monster in the file, in the binary file and in the memory  */

	if (this->printFileMonsters()) {

		unsigned int selectedId = 0;
		std::string currentLine;
		std::vector<std::string> fileLines;

		char response = 'n';

		std::cout << '\n' << "enter the id of the monster you want to update" << '\n';
		std::cin >> selectedId;

		// ------------------------ simple file ------------------------

		// read
		std::ifstream infile(this->mpath);

		while (std::getline(infile, currentLine)) {

			if (getId(currentLine) != selectedId) {

				fileLines.emplace_back(currentLine);

			} else {

				std::stringstream ss(currentLine);

				// monster variables
				unsigned int id = 0;
				std::string name;
				unsigned short hp = 0;
				unsigned short damage = 0;
				float specialAttack = 0.f;
				std::string specialAttackType;
				DateTime dt;
				unsigned int locationId = 0;
				unsigned int oldLocationId = 0;

				// set default values
				ss >> id >> name >> hp >> damage >> specialAttack >> specialAttackType >>
					dt.year >> dt.month >> dt.day >> dt.hours >> dt.minutes >> dt.seconds >> locationId;

				// reset ss
				ss.clear();
				ss.str(std::string());

				ss << id << ' ';

				// name
				std::cout << "do you want to update name? press 'y' or 'n'"<< '\n';
				std::cin >> response;
				if (response == 'y')
					std::cout << "enter new name ( string )" << '\n', std::cin >> name;

				ss << name << ' ';

				// hp
				std::cout << "do you want to update hp? press 'y' or 'n'"<< '\n';
				std::cin >> response;
				if (response == 'y')
					std::cout << "enter new hp ( unsigned short )" << '\n', std::cin >> hp;

				ss << hp << ' ';

				// damage
				std::cout << "do you want to update damage? press 'y' or 'n'"<< '\n';
				std::cin >> response;
				if (response == 'y')
					std::cout << "enter new damage ( unsigned short )" << '\n', std::cin >> damage;

				ss << damage << ' ';

				// specialAttack
				std::cout << "do you want to update specialAttack? press 'y' or 'n'"<< '\n';
				std::cin >> response;
				if (response == 'y')
					std::cout << "enter new specialAttack ( float )" << '\n', std::cin >> specialAttack;

				ss << specialAttack << ' ';

				// specialAttackType
				std::cout << "do you want to update specialAttackType? press 'y' or 'n'"<< '\n';
				std::cin >> response;

				if (response == 'y') {

					short selectedAttackType = 0;
					std::cout << "select specialAttackType ( short )" << '\n';
					for (unsigned int i = 0; i < AMOUNT_ATTACKS; i++)
						std::cout << i << " - " << ATTACK_TYPES[i] << '\n';
					std::cin >> selectedAttackType;
					specialAttackType = ATTACK_TYPES[selectedAttackType];

				}

				ss << specialAttackType << ' ';

				// dt.day
				std::cout << "do you want to update day? press 'y' or 'n'"<< '\n';
				std::cin >> response;
				if (response == 'y')
					std::cout << "enter new day ( short )" << '\n', std::cin >> dt.day;

				// dt.month
				std::cout << "do you want to update month? press 'y' or 'n'"<< '\n';
				std::cin >> response;
				if (response == 'y')
					std::cout << "enter new month ( short )" << '\n', std::cin >> dt.month;

				// dt.year
				std::cout << "do you want to update year? press 'y' or 'n'"<< '\n';
				std::cin >> response;
				if (response == 'y')
					std::cout << "enter new year ( short )" << '\n', std::cin >> dt.year;

				// dt.hours
				std::cout << "do you want to update hours? press 'y' or 'n'"<< '\n';
				std::cin >> response;
				if (response == 'y')
					std::cout << "enter new hours ( short )" << '\n', std::cin >> dt.hours;

				// dt.minutes
				std::cout << "do you want to update minutes? press 'y' or 'n'"<< '\n';
				std::cin >> response;
				if (response == 'y')
					std::cout << "enter new minutes ( short )" << '\n', std::cin >> dt.minutes;

				// dt.seconds
				std::cout << "do you want to update seconds? press 'y' or 'n'"<< '\n';
				std::cin >> response;
				if (response == 'y')
					std::cout << "enter new seconds ( short )" << '\n', std::cin >> dt.seconds;

				ss << dt.toString() << ' ';

				std::cout << "do you want to update locationId?" << '\n';
				std::cin >> response;
				if (response == 'y') {

					oldLocationId = locationId;
					this->printFileLocations();
					std::cout << "enter new locationId ( unsigned int )" << '\n';
					std::cin >> locationId;

					// update currentMonsters in locations
					if (locationId != oldLocationId) {

						std::vector<Location*> lstorage;
						this->loadLocations(lstorage);

						lstorage[getIndex(lstorage, oldLocationId)]->decrementMonsters();
						lstorage[getIndex(lstorage, locationId)]->incrementMonsters();

						this->deleteLocations(false, false);
						this->saveLocations(lstorage);

					}

				}

				ss << locationId;

				// ------------------------ memory update ------------------------
				int position = DataBase::getIndex(this->monsters, selectedId);
				if (position != -1) {

					unsigned int deletedId;

					deletedId = this->monsters[position]->getId();
					delete this->monsters[position];

					this->monsters[position] = new Monster(name, hp, damage, specialAttack, specialAttackType, dt, locationId);
					this->monsters[position]->setId(deletedId);

				}

				fileLines.emplace_back(ss.str());

			}

		}

		infile.close();

		// write
		std::ofstream outfile(this->mpath);

		for (unsigned int i = 0; i < fileLines.size() - 1; i++)
			outfile << fileLines[i] << '\n';
		outfile << fileLines.back();

		outfile.close();

		// ------------------------ binary file ------------------------

		this->saveToBinFile(fileLines);

		std::cout << "selected monster was successfully updated in file" << '\n';

	}

}

void DataBase::updateMonster(std::map<std::string, std::string>& values) {

	/* update certain monster in the file, in the binary file and in the memory */

	if (emptyFile(this->mpath)) {

		std::cout << "the file " << this->mpath << " is empty(" << '\n';
		return;

	}

	unsigned int selectedId = DataBase::stoui(values["id"]);
	std::string defaultStr = "default";
	std::string currentLine;
	std::vector<std::string> fileLines;

	// ------------------------ simple file ------------------------

	// read
	std::ifstream infile(this->mpath);

	while (std::getline(infile, currentLine)) {

		if (getId(currentLine) != selectedId) {

			fileLines.emplace_back(currentLine);

		} else {

			std::stringstream ss(currentLine);

			// monster variables
			unsigned int id = 0;
			std::string name;
			unsigned short hp = 0;
			unsigned short damage = 0;
			float specialAttack = 0.f;
			std::string specialAttackType;
			DateTime dt;
			unsigned int locationId = 0;
			unsigned int oldLocationId = 0;

			// set default values
			ss >> id >> name >> hp >> damage >> specialAttack >> specialAttackType >>
			   dt.year >> dt.month >> dt.day >> dt.hours >> dt.minutes >> dt.seconds >> locationId;

			// reset ss
			ss.clear();
			ss.str(std::string());

			if (values["name"] != defaultStr)
				name = values["name"];

			if (values["hp"] != defaultStr)
				hp = DataBase::stous(values["hp"]);

			if (values["damage"] != defaultStr)
				damage = DataBase::stous(values["damage"]);

			if (values["specialAttack"] != defaultStr)
				specialAttack = std::stof(values["specialAttack"]);

			if (values["specialAttackType"] != defaultStr)
				specialAttackType = ATTACK_TYPES[DataBase::stous(values["specialAttackType"])];

			if (values["time"] != defaultStr)
				dt.setTime(values["time"]);

			if (values["date"] != defaultStr)
				dt.setDate(values["date"]);

			if (values["locationId"] != defaultStr) {

				oldLocationId = locationId;
				locationId = DataBase::stoui(values["locationId"]);

				// update currentMonsters in locations
				if (locationId != oldLocationId) {

					std::vector<Location*> lstorage;
					this->loadLocations(lstorage);

					lstorage[getIndex(lstorage, oldLocationId)]->decrementMonsters();
					lstorage[getIndex(lstorage, locationId)]->incrementMonsters();

					this->deleteLocations(false, false);
					this->saveLocations(lstorage);

				}

			}

			ss << id << ' ' << name << ' ' << hp << ' ' << damage << ' ' << specialAttack << ' '
				<< specialAttackType << ' ' << dt.toString() << ' ' << locationId;

			// ------------------------ memory update ------------------------
			int position = DataBase::getPosition(this->monsters, selectedId);
			if (position != -1) {

				unsigned int deletedId;

				deletedId = this->monsters[position]->getId();
				delete this->monsters[position];

				this->monsters[position] = new Monster(name, hp, damage, specialAttack, specialAttackType, dt, locationId);
				this->monsters[position]->setId(deletedId);

			}

			fileLines.emplace_back(ss.str());

		}

	}

	infile.close();

	// write
	std::ofstream outfile(this->mpath);

	for (unsigned int i = 0; i < fileLines.size() - 1; i++)
		outfile << fileLines[i] << '\n';
	outfile << fileLines.back();

	outfile.close();

	// ------------------------ binary file ------------------------

	this->saveToBinFile(fileLines);

	std::cout << "selected monster was successfully updated in file" << '\n';

}

void DataBase::search() {

	/* chose the search criteria */

	short criteria;

	std::cout << "select the criteria" << '\n';
	std::cout << "0 - search for monsters by name fragment" << '\n';
	std::cout << "1 - search for monsters by hp and by damage" << '\n';
	std::cout << "2 - search by special attack type and by date / time" << '\n';
	std::cout << "3 - search by area location range, by attackType and by specialAttack" << '\n';
	std::cin >> criteria;

	if (criteria == 0) {

		std::string nameFragment;

		// input params
		std::cout << "enter fragment of name" << '\n';
		std::cin >> nameFragment;

		this->search(nameFragment);

	} else if (criteria == 1) {

		unsigned short hp;
		unsigned short damage;

		// input params
		std::cout << "enter hp" << '\n';
		std::cin >> hp;
		std::cout << "enter damage" << '\n';
		std::cin >> damage;

		this->search(hp, damage);

	} else if (criteria == 2) {

		short selectedAttackType = 0;
		std::string time;
		std::string date;
		DateTime datetime;

		// input params
		std::cout << "choose one of the following attacks" << '\n';
		for (unsigned int i = 0 ; i < AMOUNT_ATTACKS; i++)
			std::cout << i << " - " << ATTACK_TYPES[i] << '\n';
		std::cin >> selectedAttackType;

		std::cout << "enter time in format 01:34:21 ( hours.minutes.seconds )" << '\n';
		std::cin >> time;
		std::cout << "enter date in format 01.01.2001 ( day.month.year )" << '\n';
		std::cin >> date;

		datetime.setTime(time);
		datetime.setDate(date);

		this->search(selectedAttackType, datetime);

	} else if (criteria == 3) {

		double leftArea;
		double rightArea;
		short unsigned selectedAttack;
		float specialAttack;

		std::cout << "enter left area ( double )" << '\n';
		std::cin >> leftArea;

		std::cout << "enter right area ( double )" << '\n';
		std::cin >> rightArea;

		std::cout << "choose one of the following attacks ( unsigned short )" << '\n';
		for (unsigned int i = 0 ; i < AMOUNT_ATTACKS; i++)
			std::cout << i << " - " << ATTACK_TYPES[i] << '\n';
		std::cin >> selectedAttack;

		std::cout << "enter special attack ( float )" << '\n';
		std::cin >> specialAttack;

		this->search(leftArea, rightArea, selectedAttack, specialAttack);

	}

}

void DataBase::search(const std::string& nameFragment, std::vector<Monster*>* storage) {

	/* search monsters by nameFragment */

	std::ifstream infile(this->mpath);

	bool exist = false;

	// monster variables
	unsigned int id = 0;
	std::string name;
	unsigned short hp = 0;
	unsigned short damage = 0;
	float specialAttack = 0.f;
	std::string specialAttackType;
	DateTime dt;
	unsigned int locationId;

	while (infile >> id >> name >> hp >> damage >> specialAttack >> specialAttackType >>
		dt.year >> dt.month >> dt.day >> dt.hours >> dt.minutes >> dt.seconds >> locationId) {

		if (name.find(nameFragment) != std::string::npos) {

			if (!storage) {

				if (!exist)
					std::cout << "id\tname\thp\tdamage\ttime\t\tdate\t\tspAtt\t\tspAttT\n", exist = true;

				std::cout << id << '\t' << name << '\t' << hp << '\t' << damage << '\t' << dt.toFormatString() << '\t';
				std::cout << std::fixed << std::setprecision(5) << specialAttack << "\t\t" << specialAttackType << '\n';

			} else {

				storage->emplace_back(new Monster(name, hp, damage, specialAttack, specialAttackType, dt, locationId, true));
				storage->back()->setId(id);

			}

		}

	}

	if (!exist && !storage)
		std::cout << "no records found(" << '\n';

	infile.close();

}

void DataBase::search(const unsigned short _hp, const unsigned short _damage, std::vector<Monster*>* storage) {

	/* search monsters by hp and by damage */

	std::ifstream infile(this->mpath);

	bool exist = false;

	// monster variables
	unsigned int id = 0;
	std::string name;
	unsigned short hp = 0;
	unsigned short damage = 0;
	float specialAttack = 0.f;
	std::string specialAttackType;
	DateTime dt;
	unsigned int locationId;

	while (infile >> id >> name >> hp >> damage >> specialAttack >> specialAttackType >>
		dt.year >> dt.month >> dt.day >> dt.hours >> dt.minutes >> dt.seconds >> locationId) {

		if (hp >= _hp && damage <= _damage) {

			if (!storage) {

				if (!exist)
					std::cout << "id\tname\thp\tdamage\ttime\t\tdate\t\tspAtt\t\tspAttT\n", exist = true;

				std::cout << id << '\t' << name << '\t' << hp << '\t' << damage << '\t' << dt.toFormatString() << '\t';
				std::cout << std::fixed << std::setprecision(5) << specialAttack << "\t\t" << specialAttackType << '\n';

			} else {

				storage->emplace_back(new Monster(name, hp, damage, specialAttack, specialAttackType, dt, locationId, true));
				storage->back()->setId(id);

			}

		}

	}

	if (!exist && !storage)
		std::cout << "no records found" << '\n';

	infile.close();

}

void DataBase::search(const unsigned short selectedAttack, DateTime& datetime, std::vector<Monster*>* storage) {

	/* search monsters by attackType, time / date */

	std::ifstream infile(this->mpath);

	bool exist = false;

	// search variables
	std::string selectedAttackType = ATTACK_TYPES[selectedAttack];

	// monster variables
	unsigned int id = 0;
	std::string name;
	unsigned short hp = 0;
	unsigned short damage = 0;
	float specialAttack = 0.f;
	std::string specialAttackType;
	DateTime dt;
	unsigned int locationId;

	while (infile >> id >> name >> hp >> damage >> specialAttack >> specialAttackType >>
		dt.year >> dt.month >> dt.day >> dt.hours >> dt.minutes >> dt.seconds >> locationId) {

		if (dt.getTimeValue() >= datetime.getTimeValue() && dt.getDateValue() >= datetime.getDateValue() &&
			specialAttackType == selectedAttackType) {

			if (!storage) {

				if (!exist)
					std::cout << "id\tname\thp\tdamage\ttime\t\tdate\t\tspAtt\t\tspAttT\n", exist = true;

				std::cout << id << '\t' << name << '\t' << hp << '\t' << damage << '\t' << dt.toFormatString() << '\t';
				std::cout << std::fixed << std::setprecision(5) << specialAttack << "\t\t" << specialAttackType << '\n';

			} else {

				storage->emplace_back(new Monster(name, hp, damage, specialAttack, specialAttackType, dt, locationId, true));
				storage->back()->setId(id);

			}

		}

	}

	if (!exist && !storage)
		std::cout << "no records found(" << '\n';

	infile.close();

}

void DataBase::search(const double leftArea, const double rightArea, const unsigned short selectedAttack,
	float specialAttack, std::vector<std::pair<Monster*, Location*>>* storage) {

	/* search locations by area range, search monsters by attackType and by specialAttack
	 * this is pseudo INNER JOIN ))) */

	// search variables
	std::string attackType = ATTACK_TYPES[selectedAttack];

	// local memory variables
	std::vector<Location*> llocations;
	std::vector<Monster*> lmonsters;
	std::vector<std::pair<Monster*, Location*>> localMemory;

	// location variables
	unsigned int lid;
	std::string lname;
	double larea;
	unsigned int lmonstersMax;

	// monster variables
	unsigned int mid;
	std::string mname;
	unsigned short mhp;
	unsigned short mdamage;
	float mspecialAttack;
	std::string mattackType;
	DateTime mdt;
	unsigned int mlocationId;

	// location search
	std::ifstream linfile(this->lpath);

	while (linfile >> lid >> lname >> larea >> lmonstersMax) {

		if (leftArea <= larea && larea <= rightArea) {

			llocations.emplace_back(new Location(lname, larea, lmonstersMax, true));
			llocations.back()->setId(lid);

		}

	}

	linfile.close();

	// monster search
	std::ifstream minfile(this->mpath);

	while (minfile >> mid >> mname >> mhp >> mdamage >> mspecialAttack >> mattackType >>
		mdt.year >> mdt.month >> mdt.day >> mdt.hours >> mdt.minutes >> mdt.seconds >> mlocationId) {

		if (mattackType == attackType && mspecialAttack >= specialAttack &&
			DataBase::getPosition(llocations, mlocationId) == -1) {

			lmonsters.emplace_back(new Monster(mname, mhp, mdamage, mspecialAttack, mattackType, mdt, mlocationId));
			lmonsters.back()->setId(mid);

		}

	}

	minfile.close();

	// push data to local memory
	localMemory.reserve(lmonsters.size());
	for (auto& monster : lmonsters)
		localMemory.emplace_back(std::pair(monster, llocations[DataBase::getIndex(llocations, monster->getLocationId())]));

	if (storage) {

		*storage = localMemory;
		return;

	}

	if (localMemory.empty()) {

		std::cout << "no records (((" << '\n';
		return;

	}

	for (auto& p : localMemory)
		std::cout << p.first->getId() << " " << p.first->getName() << " | " <<
			p.second->getId() << " " << p.second->getName() << '\n';

	for (auto& p : localMemory) {

		delete p.first;
		delete p.second;

	}
	localMemory.clear();

}

void DataBase::bsearch(const std::string& nameFragment, std::vector<Monster*>* storage) {

	/* search monsters by nameFragment in the binary file */

	std::ifstream infile(this->mbpath, std::ios_base::binary);

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

	char ch;
	unsigned int fileEnd = 0;

	// init fileEnd
	infile.seekg(0, std::ios_base::end);
	fileEnd = infile.tellg();
	infile.seekg(0, std::ios_base::beg);

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

		if (name.find(nameFragment) != std::string::npos) {

			storage->emplace_back(new Monster(name, hp, damage, specialAttack, specialAttackType, dt, locationId, true));
			storage->back()->setId(id);

		}

		// clear strings
		name.clear();
		specialAttackType.clear();

	}

	infile.close();

}