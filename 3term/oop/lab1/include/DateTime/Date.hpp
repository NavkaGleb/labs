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
        Date(const std::tm* time) : day(time->tm_mday), month(time->tm_mon + 1), year(time->tm_year + 1900) {}
        Date(const Day& day, const Month& month, const Year& year)
            : day(day.value), month(month.value), year(year.value) {}

        // operators
        friend std::ostream& operator<<(std::ostream& stream, const Date& date) {

            stream << std::setfill('0');

            return stream << std::setw(2) << date.day << "/"
                          << std::setw(2) << date.month << "/"
                          << std::setw(4) << date.year;

        }

    }; // struct Date

}  // namespace ng