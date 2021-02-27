#ifndef LOCATION_H
#define LOCATION_H

#include "../datetime/DateTime.h"

class Location {
private:
	// variables
	static unsigned int globalId;
	unsigned int id;
	std::string name;
	double area;
	unsigned int monstersMax;
	unsigned int currentMonsters;

public:
	// constructor
	Location(const std::string& name, double area, unsigned int monstersMax, bool saved = false);

	// accessors
	const unsigned int& getId();
	const std::string& getName();
	const unsigned int& getMonstersMax();
	const unsigned int& getCurrentMonsters();

	// modifiers
	void setId(unsigned int id);
	void setCurrentMonsters(unsigned int currentMonsters);

	// methods
	static void initGlobalId(const std::string& path);
	static bool print(const std::string& path);
	void incrementMonsters();
	void decrementMonsters();
	void save(std::ofstream& outfile, bool& emptyFile);
	friend std::ostream& operator <<(std::ostream& out, const Location& location);
};

#endif // LOCATION_H
