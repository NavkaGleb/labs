#include "scheduling_system.hpp"

#include <iostream>

#include <tabulate/table.hpp>

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
  std::vector processes = { 100, 500, 40 };

  for (std::size_t i = 0; i < process_count_; ++i) {
    double x = common::R1();

    while (x == -1.0) {
      x = common::R1();
    }

    ProcessConfig process;

    process.cpu_time = (std::size_t)(x * (double)standard_process_deviation_) + average_process_time_;
    process.io_blocking = processes[i];

    processes_.emplace_back(process);
  }

  tabulate::Table table;

  table.add_row({ "cpu_time", "io_blocking", "cpu_done", "io_next", "blocked_count" });

  for (const auto& process : processes_) {
    table.add_row({
      std::to_string(process.cpu_time),
      std::to_string(process.io_blocking),
      std::to_string(process.cpu_done),
      std::to_string(process.io_next),
      std::to_string(process.blocked_count)
    });
  }

  std::cout << table << std::endl;
}

void SchedulingSystem::Run(SchedulingAlgorithm&& algorithm) {
  auto result = algorithm(simulation_time_, processes_);

  std::cout << "-------------------------" << std::endl;
  std::cout << "algorithm type: " << result.algorithm_type << std::endl;
  std::cout << "algorithm name: " << result.algorithm_name << std::endl;
  std::cout << "taken time:     " << result.taken_time << std::endl;

  tabulate::Table table;

  table.add_row({ "cpu_time", "io_blocking", "cpu_done", "io_next", "blocked_count" });

  for (const auto& process : processes_) {
    table.add_row({
      std::to_string(process.cpu_time),
      std::to_string(process.io_blocking),
      std::to_string(process.cpu_done),
      std::to_string(process.io_next),
      std::to_string(process.blocked_count)
    });
  }

  std::cout << table << std::endl;
}

} // namespace os_lab2
