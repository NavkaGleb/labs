#pragma once

#include <functional>
#include <vector>

#include "process_config.hpp"

namespace scheduler_simulator {

struct SchedulingAlgorithm {
  struct Result {
    std::string algorithm_type;
    std::string algorithm_name;
    std::size_t taken_time = 0;
  };

  virtual Result operator ()(std::size_t simulation_time, const std::vector<ProcessConfig>& process_configs) = 0;
};

} // namespace scheduler_simulator
