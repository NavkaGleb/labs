#pragma once

#include <vector>

#include "Competition.hpp"

namespace CockroachRace {

    class Race {
    public:
        explicit Race(uint32_t stepCount = 0);
        ~Race() = default;

    public:
        void TakePart(AccreditationId id);

        void Start();

    private:
        uint32_t                     m_StepCount;
        std::vector<AccreditationId> m_CockroachIds;

    }; // class Race

} // namespace CockroachRace