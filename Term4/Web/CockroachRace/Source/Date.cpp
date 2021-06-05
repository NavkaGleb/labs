#include "Date.hpp"

namespace CockroachRace {

    const Date& Date::Get() {
        static Date date = {
            GetRandomNumber(2000u, 2021u),
            GetRandomNumber(1u, 12u)
        };

        return date;
    }

} // namespace CockroachRace