#ifndef MONSTER_HPP
#define MONSTER_HPP

#include <string>
#include <cstring>
#include <iostream>
#include <random>
#include <ctime>

class Monster {
private:
	// variables
	std::string _name;														// name
	int _hp;																// health
	short _damage;															// damage
	float _achance;															// attack chance
	std::string _atype;														// attack type

public:
	// constructor
	Monster();
	Monster(const std::string& name, int hp, short damage, float achance, const std::string& atype);
	virtual ~Monster() = default;

	// accessors
	[[nodiscard]] const std::string& name() const;
	[[nodiscard]] const int& hp() const;
	[[nodiscard]] const short& damage() const;
	[[nodiscard]] const float& achance() const;
	[[nodiscard]] const std::string& atype() const;

	// methods
	friend std::ostream& operator<<(std::ostream& ostream, const Monster& monster);
	Monster& operator=(const Monster& other);

};

#endif // MONSTER_HPP
