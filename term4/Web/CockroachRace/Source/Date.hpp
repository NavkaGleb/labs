#pragma once

#include "Random.hpp"

namespace CockroachRace {

    struct Date {
        uint32_t Year;
        uint32_t Month;

        static const Date& Get();
    };

} // namespace CockroachRace