#pragma once

#include <cstdint>
#include <vector>
#include <functional>

#include "command_line_args.hpp"
#include "process_config.hpp"
#include "scheduling_algorithm.hpp"

namespace os_lab2 {

class SchedulingSystem {
 public:
  static SchedulingSystem& GetInstance();

 private:
  SchedulingSystem();

 public:
  void Init(const CommandLineArgs& args);

  template <typename ShedulingAlgorithm>
  void Run();

 private:
  std::size_t                 simulation_time_;
  std::string                 summary_results_filepath_;
  std::string                 process_results_filepath_;
  std::vector<ProcessConfig>  processes_;
};

} // namespace os_lab2

#include "scheduling_system.inl"
