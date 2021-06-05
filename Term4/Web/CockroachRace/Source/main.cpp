#include <iostream>

#include "Random.hpp"
#include "Date.hpp"
#include "Competition.hpp"
#include "Race.hpp"

CockroachRace::Cockroach GetRandomCockroach() {
    using namespace std::string_literals;

    CockroachRace::Cockroach cockroach;

    cockroach.Name        = "Cockroach_"s + CockroachRace::GetRandomNumber<char>(48, 122);

    cockroach.LegCount    = CockroachRace::GetRandomNumber(1, 10);
    cockroach.HeadCount   = CockroachRace::GetRandomNumber(2, 3);
    cockroach.EyeCount    = CockroachRace::GetRandomNumber(1, 10);

    cockroach.Experience  = CockroachRace::GetRandomNumber(0, 5);
    cockroach.Lucky       = CockroachRace::GetRandomNumber(0, 100);
    cockroach.YearOfBirth = CockroachRace::GetRandomNumber(1920u, CockroachRace::Date::Get().Year);
    cockroach.HealthState = CockroachRace::GetRandomNumber(1, 100);

    return cockroach;
}

int main() {
    std::vector<CockroachRace::AccreditationId> cockroachIds(6);

    for (auto& id : cockroachIds)
        id = CockroachRace::Competition::ApplyForAccreditation(GetRandomCockroach());

    for (auto& id : cockroachIds)
        std::cout << CockroachRace::Competition::GetCockroach(id) << std::endl;

    auto stepCount = CockroachRace::GetRandomNumber(10, 40);

    std::cout << "CurrentYear: " << CockroachRace::Date::Get().Year << std::endl;
    std::cout << "StepCount:   " << stepCount << std::endl;
    std::cout << std::endl;

    CockroachRace::Race race(stepCount);

    for (const auto& id : cockroachIds)
        race.TakePart(id);

    race.Start();

    return 0;
}
