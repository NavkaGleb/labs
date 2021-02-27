#include "DateTime.h"

// private methods

void DateTime::resetTime() {

	this->hours = 0;
	this->minutes = 0;
	this->seconds = 0;

}

void DateTime::resetDate() {

	this->day = 0;
	this->month = 0;
	this->year = 0;

}

// constructor

DateTime::DateTime() {

	this->year = 0;
	this->month = 0;
	this->day = 0;
	this->hours = 0;
	this->minutes = 0;
	this->seconds = 0;

}

// accessors

unsigned int DateTime::getTimeValue() {

	unsigned int value = 0;

	value += this->hours;
	value = value * 100 + this->minutes;
	value = value * 100 + this->seconds;

	return value;

}

unsigned int DateTime::getDateValue() {

	unsigned int value = 0;

	value += this->day;
	value = value * 100 + this->month;
	value = value * 10000 + year;

	return value;

}

// modifiers

void DateTime::setCurrentTime() {

	time_t now = time(nullptr);
	tm* curTime = localtime(&now);

	this->year = static_cast<short>(curTime->tm_year + 1900);
	this->month = static_cast<short>(curTime->tm_mon + 1);
	this->day = static_cast<short>(curTime->tm_mday);
	this->hours = static_cast<short>(curTime->tm_hour);
	this->minutes = static_cast<short>(curTime->tm_min);
	this->seconds = static_cast<short>(curTime->tm_sec);

}

void DateTime::setRandTime(std::mt19937& mt, std::uniform_int_distribution<unsigned short>& ushort_rand) {

	this->hours = static_cast<short>(ushort_rand(mt) % 24);
	this->minutes = static_cast<short>(ushort_rand(mt) % 60);
	this->seconds = static_cast<short>(ushort_rand(mt) % 60);

	this->day = static_cast<short>(ushort_rand(mt) % 30 + 1);
	this->month = static_cast<short>(ushort_rand(mt) % 12 + 1);
	this->year = static_cast<short>(ushort_rand(mt) % 121 + 1900);

}

void DateTime::setTime(const std::string& time) {

	int endHours = 2;
	int beginMinutes = 3;
	int endMinutes = 5;
	int beginSeconds = 6;
	int endSeconds = 8;

	this->resetTime();

	for (int i = 0; i < endHours; i++)
		this->hours = static_cast<short>(this->hours * 10 + short(time[i] - 48));

	for (int i = beginMinutes; i < endMinutes; i++)
		this->minutes = static_cast<short>(this->minutes * 10 + short(time[i] - 48));

	for (int i = beginSeconds; i < endSeconds; i++)
		this->seconds = static_cast<short>(this->seconds * 10 + short(time[i] - 48));

}

void DateTime::setDate(const std::string& date) {

	int endDay = 2;
	int beginMonth = 3;
	int endMonth = 5;
	int beginYear = 6;
	int endYear = 10;

	this->resetDate();

	for (int i = 0; i < endDay; i++)
		this->day = static_cast<short>(this->day * 10 + short(date[i] - 48));

	for (int i = beginMonth; i < endMonth; i++)
		this->month = static_cast<short>(this->month * 10 + short(date[i] - 48));

	for (int i = beginYear; i < endYear; i++)
		this->year = static_cast<short>(this->year * 10 + short(date[i] - 48));

}

// public methods

std::string DateTime::toString() {

	std::stringstream ss;
	ss << this->year << " " << this->month << " " << this->day << " ";
	ss << this->hours << " " << this->minutes << " " << this->seconds;

	return ss.str();

}

std::string DateTime::toFormatString() {

	std::stringstream ss;
	ss << std::setfill('0');
	ss << std::setw(2) << this->hours << ':' << std::setw(2) << this->minutes << ':' << std::setw(2) << this->seconds << '\t';
	ss << std::setw(2) <<  this->day << '.' << std::setw(2) << this->month << '.' << this->year;

	return ss.str();

}