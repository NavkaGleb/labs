#ifndef MONSTER_H
#define MONSTER_H

#include "../location/Location.h"

// constants

static const unsigned int AMOUNT_ATTACKS = 4;
static const char* ATTACK_TYPES[AMOUNT_ATTACKS] = { "increase_damage", "repeat_attack", "cure_yourself", "paralyze_an_enemy" };

static const char SYMBOLS[] = { "ABCDEFGHIGKMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789@_" };
static const unsigned short SYMBOL_AMOUNT = sizeof(SYMBOLS) - 1;

class Monster {
private:
	// variables
	static unsigned int globalId;
	unsigned int id;
	std::string name;
	unsigned short hp;
	unsigned short damage;
	float specialAttack;
	std::string specialAttackType;
	DateTime datetime;
	unsigned int locationId;
	bool infile;

public:
	// constructor
	Monster(const std::string& name, unsigned short hp, unsigned short damage, float specialAttack,
		const std::string& specialAttackType, DateTime datetime, unsigned int locationId, bool infile = false);

	// accessors
	const unsigned int& getId();
	const std::string& getName();
	const unsigned int& getLocationId();
	const bool& getInfile();

	// modifiers
	void setId(unsigned int id);
	void setLocationId(unsigned int locationId);
	void setInfile(bool infile);

	// methods
	static void initGlobalId(const std::string& path);
	static bool print(const std::string& path);
	static bool bprint(const std::string& path);
	void saveToFile(std::ofstream& outfile, bool& emptyFile, bool printResponse = false);
	void saveToBinFile(std::ofstream& outfile, bool printResponse = false);

	friend std::ostream& operator <<(std::ostream& out, Monster& monster);

};

#endif // MONSTER_H
