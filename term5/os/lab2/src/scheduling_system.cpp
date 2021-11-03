#include "scheduling_system.hpp"

#include <iostream>

#include "common.hpp"

namespace os_lab2 {

SchedulingSystem& SchedulingSystem::GetInstance() {
  static SchedulingSystem instance;
  return instance;
}

SchedulingSystem::SchedulingSystem()
  : process_count_(3)
  , average_process_time_(1100)
  , standard_process_deviation_(510)
  , simulation_time_(10'000) {}

void SchedulingSystem::Init(CommandLineArgs args) {
  std::vector processes = { 100, 500, 200 };

  for (std::size_t i = 0; i < process_count_; ++i) {
    double x = common::R1();

    while (x == -1.0) {
      x = common::R1();
    }

    Process process;

    process.cpu_time = (std::size_t)(x * (double)standard_process_deviation_) + average_process_time_;
    process.io_blocking = processes[i];

    processes_.emplace_back(process);
  }

  for (const auto& process : processes_) {
    std::cout << process.ToString() << std::endl;
  }
}

void SchedulingSystem::Run(SchedulingAlgorithm&& algorithm) {
  auto result = algorithm(simulation_time_, processes_);

  std::cout << "algorithm type: " << result.algorithm_type << std::endl;
  std::cout << "algorithm name: " << result.algorithm_name << std::endl;
  std::cout << "taken time:     " << result.taken_time << std::endl;
}

} // namespace os_lab2
