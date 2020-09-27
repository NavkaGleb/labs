#pragma once

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

    }; // struct Date

}  // namespace ng