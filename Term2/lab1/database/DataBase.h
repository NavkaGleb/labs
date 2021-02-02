#ifndef DATABASE_H
#define DATABASE_H

#include "../entities/Monster.h"

class DataBase {
private:
	// variables
	// filepath
	std::string mpath;	 					// monsters path
	std::string mbpath;						// monsters binary path
	std::string lpath;						// locations path
	std::string modesPath;
	std::string commands1Path;
	std::string commands2Path;

	// memory
	std::vector<Monster*> monsters;
	std::vector<Location*> locations;

	// commands
	std::vector<std::string> modes;
	std::vector<std::string> commands1;
	std::vector<std::string> commands2;

	// random
	std::random_device rd;
	std::mt19937 mt;
	std::uniform_int_distribution<unsigned short> ushort_rand;
	std::uniform_real_distribution<float> float_rand;
	std::uniform_real_distribution<double> double_rand;

	// methods
	static void initMenu(const std::string& path, std::vector<std::string>& menu);

	static unsigned int getId(const std::string& str);
	static unsigned int getLocationId(const std::string& str);
	static bool emptyFile(const std::string& path);

	template <typename T>
	static int getIndex(std::vector<T>& data, unsigned int id);
	template <typename T>
	static int getPosition(std::vector<T>& data, unsigned int id);

	void saveToBinFile(const std::vector<std::string>& data);
	static unsigned int stoui(const std::string& str);
	static unsigned short stous(const std::string& str);

public:
	// constructor
	DataBase(const std::string& mpath, const std::string& mbpath, const std::string& lPath);

	// accessors
	std::vector<std::string>& getModes();
	std::vector<std::string>& getCommands1();
	std::vector<std::string>& getCommands2();
	std::vector<Location*>& getLocations();

	// methods
	static void print(const std::vector<std::string>& commands);

	// ------------------------- Locations -------------------------

	bool printFileLocations();
	bool printMemoryLocations();

	void createLocation();
	void createLocation(const std::string& name, double area, unsigned int monstersMax);
	void createLocations(bool showResponse = false, unsigned int amount = 0);

	void loadLocations(bool showResponse = false);
	bool loadLocations(std::vector<Location*>& storage, bool showResponse = false);

	void saveLocations(std::vector<Location*>& storage, bool showResponse = false);

	void deleteLocations(bool deleteMonsters, bool showResponse = false);
	static void deleteLocationsFromMemory(std::vector<Location*>& storage, bool showResponse = false);

	// ------------------------- Monsters -------------------------

	bool printFileMonsters();
	bool printBinMonsters();
	bool printMemoryMonsters();

	void createMonster();
	void createMonster(const std::string& name, unsigned short hp, unsigned short damage, float specialAttack,
		short selectedAttackType, unsigned int locationId);
	bool createMonsters(bool showResponse = false, unsigned int amount = 0);

	unsigned long long loadMonsters(bool showResponse = false);
	unsigned long long loadBinMonsters(bool showResponse = false);

	void saveMonstersToFile(bool showResponse);
	void saveMonstersToBinFile(bool showResponse);
	void saveMonsters(bool showResponse = false);

	void deleteMonsterFromFile();
	void deleteMonsterFromFile(unsigned int id);
	void deleteMonsterFromMemory(bool showResponse = false, int index = -1);

	void deleteMonstersFromFile(bool showResponse);
	void deleteMonstersFromBinFile(bool showResponse);
	void deleteMonsters(bool file, bool bfile, bool showResponse = false);
	void deleteMonstersFromMemory(bool showResponse = false);

	void updateMonster();
	void updateMonster(std::map<std::string, std::string>& values);

	void search();
	void search(const std::string& nameFragment, std::vector<Monster*>* storage = nullptr);
	void search(unsigned short hp, unsigned short damage, std::vector<Monster*>* storage = nullptr);
	void search(unsigned short selectedAttack, DateTime& datetime, std::vector<Monster*>* storage = nullptr);
	void search(double leftArea, double rightArea, unsigned short selectedAttack,
		float specialAttack, std::vector<std::pair<Monster*, Location*>>* store = nullptr);

	void bsearch(const std::string& nameFragment, std::vector<Monster*>* storage = nullptr);

};

#endif // DATABASE_H