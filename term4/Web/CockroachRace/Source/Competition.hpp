#pragma once

#include <unordered_map>

#include "Cockroach.hpp"

namespace CockroachRace {

    using AccreditationId = uint32_t;

    class Competition {
    public:
        Competition() = default;
        ~Competition() = default;

    public:
        [[nodiscard]] static Cockroach& GetCockroach(AccreditationId id);

        [[nodiscard]] static AccreditationId ApplyForAccreditation(const Cockroach& props);

    private:
        struct Data {
            AccreditationId                                CurrentAccreditationId = 0;
            std::unordered_map<AccreditationId, Cockroach> Cockroaches;
        };

    private:
        static Data& GetData();

    }; // class Competition

} // namespace CockroachRace
