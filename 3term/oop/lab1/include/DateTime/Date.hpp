#pragma once

#include <ostream>
#include <iomanip>
#include <ctime>

namespace ng {

    struct Day {
        // variables
        short value;

        // constructor
        explicit Day(short value) : value(value) {}

    }; // struct Day

    struct Month {
        // variables
        short value;

        // constructor
        explicit Month(short value) : value(value) {}

    }; // struct Month

    struct Year {
        // variables
        short value;

        // constructor
        explicit Year(short value) : value(value) {}

    }; // struct Year

    struct Date {
        // variables
        short day;
        short month;
        short year;

        // constructor
        Date() : day(0), month(0), year(0) {}
        Date(const Day& day, const Month& month, const Year& year)
            : day(day.value), month(month.value), year(year.value) {}
        Date(const std::time_t* timeT) {

            std::tm* time = std::localtime(timeT);

            this->day = time->tm_mday;
            this->month = time->tm_mon + 1;
            this->year = time->tm_year + 1900;

        }

        // operators
        bool operator<(const Date& other) const {

            if (this->year != other.year)
                return this->year < other.year;

            if (this->month != other.month)
                return this->month < other.month;

            return this->day < other.day;

        }

        bool operator>(const Date& other) const {

            if (this->year != other.year)
                return this->year > other.year;

            if (this->month != other.month)
                return this->month > other.month;

            return this->day > other.day;

        }

        bool operator<=(const Date& other) const {

            if (this->year != other.year)
                return this->year < other.year;

            if (this->month != other.month)
                return this->month < other.month;

            return this->day <= other.day;

        }

        bool operator>=(const Date& other) const {

            if (this->year != other.year)
                return this->year > other.year;

            if (this->month != other.month)
                return this->month > other.month;

            return this->day >= other.day;

        }

        bool operator!=(const Date& other) const {

            if (this->year != other.year)
                return true;

            if (this->month != other.month)
                return true;

            return this->day != other.day;

        }

        bool operator==(const Date& other) const {

            if (this->year != other.year)
                return false;

            if (this->month != other.month)
                return false;

            return this->day == other.day;

        }
        
        friend std::ostream& operator<<(std::ostream& stream, const Date& date) {

            stream << std::setfill('0');

            return stream << std::setw(2) << date.day << "/"
                          << std::setw(2) << date.month << "/"
                          << std::setw(4) << date.year;

        }

    }; // struct Date

}  // namespace ng