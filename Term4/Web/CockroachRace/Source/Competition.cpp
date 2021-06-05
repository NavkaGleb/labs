#include "Competition.hpp"

namespace CockroachRace {

    Cockroach& Competition::GetCockroach(AccreditationId id) {
        return GetData().Cockroaches[id];
    }

    AccreditationId Competition::ApplyForAccreditation(const Cockroach& props) {
        GetData().Cockroaches[GetData().CurrentAccreditationId] = props;

        return GetData().CurrentAccreditationId++;
    }

    Competition::Data& Competition::GetData() {
        static Data data;
        return data;
    }

} // namespace CockroachRace