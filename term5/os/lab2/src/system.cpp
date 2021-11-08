#include "system.hpp"

#include <iostream>
#include <fstream>

#include <ng_random/random.hpp>

#include <tabulate/table.hpp>

#include <nlohmann/json.hpp>

namespace os_lab2 {

System& System::GetInstance() {
  static System instance;
  return instance;
}

System::System()
  : simulation_time_(0) {}

void System::Init(const CommandLineArgs& args) {
  if (args.GetCount() != 3) {
    throw std::invalid_argument("Didn't provided file with config!");
  }

  results_filepath_ = args[2];

  std::ifstream   in_file(args[1]);
  nlohmann::json  json;

  in_file >> json;

  in_file.close();

  simulation_time_ = json["simulation_time"];

  processes_.reserve(json["process_config"].size());

  tabulate::Table table;

  table.add_row({ "name", "user_id", "max_cpu_time", "io_block_time", "io_block_period",  });

  for (const auto& json_process_config : json["process_config"]) {
    ProcessConfig process_config;

    process_config.name     = json_process_config["name"];
    process_config.user_id  = json_process_config["user_id"];

    Range<std::size_t> cpu_time = {
      json_process_config["cpu_time"]["average"],
      json_process_config["cpu_time"]["deviation"]
    };

    process_config.io_block_time = {
      json_process_config["io_block_time"]["average"],
      json_process_config["io_block_time"]["deviation"]
    };

    process_config.io_block_period = {
      json_process_config["io_block_period"]["average"],
      json_process_config["io_block_period"]["deviation"]
    };

    process_config.max_cpu_time = ng::random::Next<std::size_t>(cpu_time.average - cpu_time.deviation, cpu_time.average + cpu_time.deviation);

    tabulate::Table io_block_time_table;
    io_block_time_table.add_row({
      std::to_string(process_config.io_block_time.average),
      std::to_string(process_config.io_block_time.deviation)
    }).format().hide_border();

    tabulate::Table io_block_period_table;
    io_block_period_table.add_row({
      std::to_string(process_config.io_block_period.average),
      std::to_string(process_config.io_block_period.deviation)
    }).format().hide_border();

    table.add_row({
      process_config.name,
      std::to_string(process_config.user_id),
      std::to_string(process_config.max_cpu_time),
      io_block_time_table,
      io_block_period_table
    });

    processes_.push_back(process_config);
  }

  std::cout << table << std::endl;

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

  table.add_row({ "cpu_time", "io_blocking", "cpu_done" });
  table.row(0).format().font_color(tabulate::Color::yellow);

  for (const auto& process : processes_) {
    table.add_row({
      std::to_string(process.max_cpu_time),
      std::to_string(process.io_block_time.average),
      std::to_string(process.max_cpu_time),
    });
  }

  std::cout << table << std::endl;
  out_file << table << std::endl;

  out_file.close();

  std::cout << "Finish!" << std::endl;
}

} // namespace os_lab2
