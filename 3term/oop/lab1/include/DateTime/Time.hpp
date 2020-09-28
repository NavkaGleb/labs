#pragma once

#include <ostream>
#include <iomanip>
#include <ctime>

namespace ng {

    struct Hour {
        // variables
        short value;

        // constructor
        explicit Hour(short value) : value(value) {}

    };

    struct Time {
        // variables
        short hour;
        short minute;
        short second;

        // constructor
        Time() : hour(0), minute(0), second(0) {}
        Time(const std::tm* time) : hour(time->tm_hour), minute(time->tm_min), second(time->tm_sec) {}
        Time(short hour, short minute, short second) : hour(hour), minute(minute), second(second) {}

        // operators
        friend std::ostream& operator<<(std::ostream& stream, const Time& time) {

            stream << std::setfill('0');

            return stream << std::setw(2) << time.hour << "."
                          << std::setw(2) << time.minute << "."
                          << std::setw(2) << time.second;

        }

    };

} // namespace ng