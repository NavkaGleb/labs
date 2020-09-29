#pragma once

#include "Time.hpp"
#include "Date.hpp"

namespace ng {

    struct DateTime {
        // variables
        Time time;
        Date date;

        // constructor
        DateTime() : time(), date() {}
        DateTime(const Time& time, const Date& date) : time(time), date(date) {}
        DateTime(const std::time_t* timeT) {

            std::tm* ctime = std::localtime(timeT);

            this->time = Time(Hour(ctime->tm_hour), Minute(ctime->tm_min), Second(ctime->tm_sec));
            this->date = Date(Day(ctime->tm_mday), Month(ctime->tm_mon + 1), Year(ctime->tm_year + 1900));

        }

        bool operator<(const DateTime& other) {

            if (this->date != other.date)
                return this->date < other.date;

            return this->time < other.time;

        }

        bool operator>(const DateTime& other) {

            if (this->date != other.date)
                return this->date > other.date;

            return this->time > other.time;

        }

        bool operator<=(const DateTime& other) {

            if (this->date != other.date)
                return this->date < other.date;

            return this->time <= other.time;

        }

        bool operator>=(const DateTime& other) {

            if (this->date != other.date)
                return this->date > other.date;

            return this->time >= other.time;

        }

        bool operator!=(const DateTime& other) {

            if (this->date != other.date)
                return true;

            return this->time != other.time;

        }

        bool operator==(const DateTime& other) {

            if (this->date != other.date)
                return false;

            return this->time == other.time;

        }

        friend std::ostream& operator<<(std::ostream& stream, const DateTime& dt) {

            return stream << dt.time << " " << dt.date;

        }

    };

}