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

    struct Minute {
        // variables
        short value;

        // constructor
        explicit Minute(short value) : value(value) {}

    };

    struct Second {
        // variables
        short value;

        // constructor
        explicit Second(short value) : value(value) {}

    };

    struct Time {
        // variables
        short hour;
        short minute;
        short second;

        // constructor
        Time() : hour(0), minute(0), second(0) {}
        Time(const Hour& hour, const Minute& minute, const Second& second)
            : hour(hour.value), minute(minute.value), second(second.value) {}
        Time(const std::time_t* timeT) {

            std::tm* time = std::localtime(timeT);

            this->hour = time->tm_hour;
            this->minute = time->tm_min;
            this->second = time->tm_sec;

        }

        // operators
        bool operator<(const Time& other) const {

            if (this->hour != other.hour)
                return this->hour < other.hour;

            if (this->minute != other.minute)
                return this->minute < other.minute;

            return this->second < other.second;

        }

        bool operator>(const Time& other) const {

            if (this->hour != other.hour)
                return this->hour > other.hour;

            if (this->minute != other.minute)
                return this->minute > other.minute;

            return this->second > other.second;

        }

        bool operator<=(const Time& other) const {

            if (this->hour != other.hour)
                return this->hour < other.hour;

            if (this->minute != other.minute)
                return this->minute < other.minute;

            return this->second <= other.second;

        }

        bool operator>=(const Time& other) const {

            if (this->hour != other.hour)
                return this->hour > other.hour;

            if (this->minute != other.minute)
                return this->minute > other.minute;

            return this->second >= other.second;

        }

        bool operator!=(const Time& other) const {

            if (this->hour != other.hour)
                return true;

            if (this->minute != other.minute)
                return true;

            return this->second != other.second;

        }

        bool operator==(const Time& other) const {

            if (this->hour != other.hour)
                return false;

            if (this->minute != other.minute)
                return false;

            return this->second == other.second;

        }

        friend std::ostream& operator<<(std::ostream& stream, const Time& time) {

            stream << std::setfill('0');

            return stream << std::setw(2) << time.hour << "."
                          << std::setw(2) << time.minute << "."
                          << std::setw(2) << time.second;

        }

    };

} // namespace ng