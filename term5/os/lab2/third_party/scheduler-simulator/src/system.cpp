#include "system.hpp"

#include <iostream>
#include <fstream>

#include <tabulate/table.hpp>

#include "common.hpp"

namespace scheduler_simulator {

System& System::GetInstance() {
  static System instance;
  return instance;
}

System::System()
  : simulation_time_(0) {}

void System::Init(const CommandLineArgs& args) {
  struct Config {
    std::size_t process_count               = 0;
    std::size_t average_process_time        = 0;
    std::size_t standard_process_deviation  = 0;
  };

  if (args.GetCount() != 3) {
    throw std::invalid_argument("Didn't provided file with config!");
  }

  results_filepath_ = args[2];

  Config        config;
  std::ifstream in_file(args[1]);
  std::string   fragment;

  while (in_file >> fragment) {
    if (fragment == "process_count") {
      in_file >> config.process_count;

      processes_.resize(config.process_count);
    }

    if (fragment == "average_process_time") {
      in_file >> config.average_process_time;
    }

    if (fragment == "standard_process_deviation") {
      in_file >> config.standard_process_deviation;
    }

    if (fragment == "io_blocking") {
      for (auto& process : processes_) {
        double x = common::R1();

        while (x == -1.0) {
          x = common::R1();
        }

        process.cpu_time = (std::size_t)(x * (double)config.standard_process_deviation) + config.average_process_time;

        in_file >> process.io_blocking;
      }
    }

    if (fragment == "simulation_time") {
      in_file >> simulation_time_;
    }
  }

  in_file.close();

  std::cout << "Initialization was successful!" << std::endl;
}

void System::Run(SchedulingAlgorithm&& algorithm) {
  std::cout << "Working..." << std::endl;

  auto result = algorithm(simulation_time_, processes_);

  std::ofstream out_file(results_filepath_);

  out_file << "algorithm type: " << result.algorithm_type << std::endl;
  out_file << "algorithm name: " << result.algorithm_name << std::endl;
  out_file << "taken time:     " << result.taken_time << std::endl;

  tabulate::Table table;

  table.add_row({ "cpu_time", "io_blocking", "cpu_done", "block_count" });
  table.row(0).format().font_color(tabulate::Color::yellow);

  for (const auto& process : processes_) {
    table.add_row({
      std::to_string(process.cpu_time),
      std::to_string(process.io_blocking),
      std::to_string(process.cpu_done),
      std::to_string(process.block_count)
    });
  }

  std::cout << table << std::endl;
  out_file << table << std::endl;

  out_file.close();

  std::cout << "Finish!" << std::endl;
}

} // namespace scheduler_simulator
