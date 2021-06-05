#pragma once

#include <cstdint>
#include <string>
#include <ostream>

namespace CockroachRace {

    struct Cockroach {
        std::string Name        = "DummyCockroach";

        uint32_t    LegCount    = 0;
        uint32_t    HeadCount   = 0;
        uint32_t    EyeCount    = 0;

        uint32_t    Experience  = 0;
        uint32_t    Lucky       = 0; // 0 - 100
        uint32_t    YearOfBirth = 0;
        uint32_t    HealthState = 0; // 0 - 100
    };

    inline std::ostream& operator <<(std::ostream& out, const Cockroach& cockroach) {
        out << "Name:        " << cockroach.Name        << std::endl;
        out << "LegCount:    " << cockroach.LegCount    << std::endl;
        out << "HeadCount:   " << cockroach.HeadCount   << std::endl;
        out << "EyeCount:    " << cockroach.EyeCount    << std::endl;
        out << "Experience:  " << cockroach.Experience  << std::endl;
        out << "Lucky:       " << cockroach.Lucky       << std::endl;
        out << "YearOfBirth: " << cockroach.YearOfBirth << std::endl;
        out << "HealthState: " << cockroach.HealthState << std::endl;

        return out;
    }

} // namespace CockroachRace