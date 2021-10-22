#include "Monster.hpp"

// constructor

Monster::Monster() {

	// static variables
	static std::mt19937 mt(std::clock());
	static std::uniform_int_distribution<int> irand(0, 50000);
	static std::uniform_real_distribution<float> frand(0.f, 1.f);
	static const char* atypes[] = { "increase damage", "increase damage", "cure yourself", "paralyze an enemy" };
	static const char* symbols = "ABCDEFGHIGKMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789@_";

	for (int i = 0; i < 10; i++)
		this->_name += symbols[irand(mt) % std::strlen(symbols)];

	this->_hp = irand(mt);
	this->_damage = static_cast<short>(irand(mt));
	this->_achance = frand(mt);
	this->_atype = atypes[irand(mt) % 4];

}

Monster::Monster(const std::string& name, int hp, short damage, float achance, const std::string& atype) {

	this->_name = name;
	this->_hp = hp;
	this->_damage = damage;
	this->_achance = achance;
	this->_atype = atype;

}

// accessors

const std::string& Monster::name() const {

	return this->_name;

}

const int& Monster::hp() const {

	return this->_hp;

}

const short& Monster::damage() const {

	return this->_damage;

}

const float& Monster::achance() const {

	return this->_achance;

}

const std::string& Monster::atype() const {

	return this->_atype;

}

// methods

std::ostream& operator<<(std::ostream& ostream, const Monster& monster)  {

	ostream << monster._name << ": " << monster._hp << ", " << monster._damage << ", " << monster._achance << ", " << monster._atype;
	return ostream;

}

Monster& Monster::operator=(const Monster& other) {

	if (this == &other)
		return *this;

	this->_name = other._name;
	this->_hp = other._hp;
	this->_damage = other._damage;
	this->_achance = other._achance;
	this->_atype = other._atype;

	return *this;

}