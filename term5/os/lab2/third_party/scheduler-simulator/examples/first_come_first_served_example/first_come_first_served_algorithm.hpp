#pragma once

#include <scheduler_simulator/scheduling_algorithm.hpp>

struct FirstComeFirstServedAlgorithm : public scheduler_simulator::SchedulingAlgorithm {
  Result operator ()(std::size_t simulation_time, std::vector<scheduler_simulator::ProcessConfig>& processes) override;
};
