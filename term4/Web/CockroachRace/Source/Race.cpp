#include "Race.hpp"

#include <iostream>
#include <fstream>

#include "Random.hpp"
#include "Date.hpp"

namespace CockroachRace {

    Race::Race(uint32_t stepCount)
        : m_StepCount(std::max(10u, std::min(40u, stepCount))) {
    }

    void Race::TakePart(AccreditationId id) {
        m_CockroachIds.push_back(id);
    }

    void Race::Start() {
        static const auto GetStepDistance = [&](AccreditationId id) {
            // LegCount
            // HeadCount
            // EyeCount

            // Experience
            // Lucky
            // YearOfBirth
            // HealthState

            float result = 1.0f;

            auto& cockroach = Competition::GetCockroach(id);

            if (CockroachRace::GetRandomNumber(1, 20) == 7) {   // died by accident
                cockroach.LegCount = 0;
                return 0.0f;
            }

            if (CockroachRace::GetRandomNumber(1, 20) == 6 && cockroach.HeadCount >= 2)
                --cockroach.HeadCount;

            result *= static_cast<float>(cockroach.LegCount)  / 3.0f;
            result *= static_cast<float>(cockroach.HeadCount) / 2.0f;
            result *= static_cast<float>(cockroach.EyeCount)  / 3.0f;

            if      (cockroach.Experience == 5) result *= 1.5f;
            else if (cockroach.Experience >= 3) result *= 1.3f;
            else if (cockroach.Experience >= 1) result *= 1.2f;

            result *= static_cast<float>(cockroach.Lucky) / 40.0f;

            auto age = Date::Get().Year - cockroach.YearOfBirth;

            if      (age < 14) result *= 0.8f;
            else if (age < 26) result *= 1.5f;
            else if (age < 40) result *= 0.9f;
            else if (age < 50) result *= 0.85f;
            else if (age < 70) result *= 0.7f;
            else               return 0.0f;

            result *= static_cast<float>(cockroach.HealthState) / 40.0f;

            if (result == 0.0f)
                cockroach.LegCount = 0;

            if (result > 1.0f)
                return 1.0f;

            if (cockroach.Lucky >= 20 && cockroach.Lucky <= 80)
                cockroach.Lucky += CockroachRace::GetRandomNumber(-10, 10);

            if (cockroach.HealthState >= 1)
                --cockroach.HealthState;

            return result;
        };

        std::ofstream outfile("../../Assets/race.json");

        outfile << "{" << std::endl;
        outfile << "\t" << R"("race": {)" << std::endl;
        {
            outfile << "\t\t" << R"("stepCount": )" << m_StepCount << "," << std::endl;

            outfile << "\t\t" << R"("ids": [)" << std::endl;
            {
                outfile << "\t\t\t";
                for (auto cockroachId : m_CockroachIds) {
                    outfile << cockroachId;

                    outfile << (cockroachId == m_CockroachIds.back() ? "" : ", ");
                }
                outfile << std::endl;
            }
            outfile << "\t\t]," << std::endl;

            outfile << "\t\t" << R"("cockroaches": [)" << std::endl;
            {
                for (auto cockroachId : m_CockroachIds) {
                    outfile << "\t\t\t{" << std::endl;
                    {
                        outfile << "\t\t\t\t" << R"("name": )" << "\"" << Competition::GetCockroach(cockroachId).Name << "\"" << std::endl;
                    }
                    outfile << "\t\t\t}" << (cockroachId == m_CockroachIds.back() ? "" : ",") << std::endl;
                }
            }
            outfile << "\t\t" << "]," << std::endl;

            outfile << "\t\t" << R"("steps": [)" << std::endl;
            {
                for (uint32_t i = 0; i < m_StepCount; ++i) {
                    outfile << "\t\t\t{" << std::endl;
                    {
                        outfile << "\t\t\t\t" << R"("distances": [)" << std::endl;
                        {
                            for (auto id : m_CockroachIds) {
                                outfile << "\t\t\t\t\t{" << std::endl;
                                {
                                    auto distance = GetStepDistance(id);

                                    outfile << "\t\t\t\t\t\t" << R"("id": )" << id << "," << std::endl;
                                    outfile << "\t\t\t\t\t\t" << R"("distance": )" << distance << std::endl;

                                    std::cout << "Id: " << id << ", Step: " << distance << std::endl;
                                }
                                outfile << "\t\t\t\t\t}" << (id == m_CockroachIds.back() ? "" : ",") << std::endl;
                            }
                            std::cout << std::endl;
                        }
                        outfile << "\t\t\t\t" << "]" << std::endl;
                    }
                    outfile << "\t\t\t}" << (i == m_StepCount - 1 ? "" : ",") << std::endl;
                }
            }
            outfile << "\t\t" << "]" << std::endl;
        }
        outfile << "\t}" << std::endl;
        outfile << "}" << std::endl;

        outfile.close();
    }

} // namespace CockroachRace