#pragma once

#include "scheduling_algorithm.hpp"

namespace os_lab2 {

struct FirstComeFirstServedAlgorithm : public SchedulingAlgorithm {
  Result operator ()(std::size_t simulation_time, std::vector<ProcessConfig>& processes) override;
};

} // namespace os_lab2
