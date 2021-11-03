#pragma once

#include <cstdint>
#include <vector>
#include <functional>

#include "command_line_args.hpp"
#include "process.hpp"

namespace os_lab2 {

struct SchedulingAlgorithmResult {
  std::string algorithm_type;
  std::string algorithm_name;
  std::size_t taken_time     = 0;
};

using SchedulingAlgorithm = std::function<SchedulingAlgorithmResult(std::size_t, std::vector<Process>&)>;

class SchedulingSystem {
 public:
  static SchedulingSystem& GetInstance();

 private:
  SchedulingSystem();

 public:
  void Init(CommandLineArgs args);
  void Run(SchedulingAlgorithm&& algorithm);

 private:
  std::size_t process_count_;
  std::size_t average_process_time_;
  std::size_t standard_process_deviation_;
  std::size_t simulation_time_;

  std::vector<Process> processes_;
};

} // namespace os_lab2
