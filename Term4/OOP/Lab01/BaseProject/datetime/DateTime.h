#ifndef DATETIME_H
#define DATETIME_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include <vector>
#include <iomanip>
#include <random>
#include <climits>
#include <map>
#include <chrono>
#include <random>

class DateTime {
private:
	// methods
	void resetTime();
	void resetDate();

public:
	// variables
	short year;
	short month;
	short day;
	short hours;
	short minutes;
	short seconds;

	// constructor
	DateTime();

	// accessors
	unsigned int getTimeValue();
	unsigned int getDateValue();

	// modifiers
	void setCurrentTime();
	void setRandTime(std::mt19937& mt, std::uniform_int_distribution<unsigned short>& ushort_rand);
	void setTime(const std::string& time);
	void setDate(const std::string& date);

	// methods
	std::string toString();
	std::string toFormatString();

};

#endif // DATETIME_H
