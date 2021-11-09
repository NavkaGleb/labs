#pragma once

#include <functional>
#include <vector>

#include "process_config.hpp"

namespace os_lab2 {

struct SchedulingAlgorithm {
  struct Result {
    std::string algorithm_type;
    std::string algorithm_name;
    std::size_t taken_time = 0;
  };

  struct Config {
    std::size_t                       simulation_time = 0;
    const std::vector<ProcessConfig>& process_configs;
    const std::string&                process_results_filepath;
  };

  virtual Result operator ()(Config&& config) = 0;
};

} // namespace os_lab2
