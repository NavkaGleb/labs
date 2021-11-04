#pragma once

#include <scheduler_simulator/scheduling_algorithm.hpp>

namespace os_lab2 {

struct FairShareAlgorithm : public scheduler_simulator::SchedulingAlgorithm {
  Result operator ()(std::size_t simulation_time, std::vector<scheduler_simulator::ProcessConfig>& processes) override;
};

} // namespace os_lab2
