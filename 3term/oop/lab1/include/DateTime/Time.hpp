#pragma once

namespace ng {

    struct Time {
        // variables
        short second;
        short minute;
        short hour;

        // constructor
        Time() : second(0), minute(0), hour(0) {}
        Time(short second, short minute, short hour) : second(second), minute(minute), hour(hour) {}

    };

} // namespace ng