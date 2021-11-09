#include "scheduling_system.hpp"

#include <tabulate/table.hpp>

#include <nlohmann/json.hpp>

namespace os_lab2 {

SchedulingSystem& SchedulingSystem::GetInstance() {
  static SchedulingSystem instance;
  return instance;
}

SchedulingSystem::SchedulingSystem()
  : simulation_time_(0) {}

void SchedulingSystem::Init(const CommandLineArgs& args) {
  if (args.GetCount() != 4) {
    throw std::invalid_argument("Didn't provided file with config!");
  }

  summary_results_filepath_ = args[2];
  process_results_filepath_ = args[3];

  std::cout << args[1] << std::endl;

  std::ifstream   in_file(args[1]);
  nlohmann::json  json;

  in_file >> json;

  in_file.close();

  if (!json.contains("simulation_time")) {
    throw std::invalid_argument("Config file does not meet requirements: No 'simulation_time' field");
  }

  if (!json.contains("process_config")) {
    throw std::invalid_argument("Config file does not meet requirements: No 'process_config' field");
  }

  if (json["process_config"].empty()) {
    throw std::invalid_argument("Config file does not meet requirements: 'process_config' has to contains at least one process");
  }

  simulation_time_ = json["simulation_time"];

  processes_.reserve(json["process_config"].size());

  tabulate::Table table;

  table.add_row({ "name", "user_id", "cpu_time_range", "io_block_range" }).row(0).format()
    .font_color(tabulate::Color::yellow);

  for (const auto& json_process_config : json["process_config"]) {
    ProcessConfig process_config;

    if (!json_process_config.contains("name")) {
      throw std::invalid_argument("Config file does not meet requirements: No 'name' in 'process_config'");
    }

    if (!json_process_config.contains("user_id")) {
      throw std::invalid_argument("Config file does not meet requirements: No 'user_id' in 'process_config'");
    }

    if (!json_process_config.contains("cpu_time_range")) {
      throw std::invalid_argument("Config file does not meet requirements: No 'cpu_time_range' in 'process_config'");
    }

    process_config.name     = json_process_config["name"];
    process_config.user_id  = json_process_config["user_id"];

    process_config.cpu_time_range = {
      json_process_config["cpu_time_range"]["min"],
      json_process_config["cpu_time_range"]["max"]
    };

    if (json_process_config.contains("io_block_range")) {
      process_config.io_block_range = {
        json_process_config["io_block_range"]["min"],
        json_process_config["io_block_range"]["max"]
      };
    }

    tabulate::Table io_block_period_table;

    if (process_config.io_block_range) {
      io_block_period_table.add_row({
        std::to_string(process_config.io_block_range->min) + ":" +
        std::to_string(process_config.io_block_range->max)
      }).format().hide_border();
    } else {
      io_block_period_table.add_row({ "-" }).format().hide_border();
    }

    table.add_row({
      process_config.name,
      std::to_string(process_config.user_id),
      std::to_string(process_config.cpu_time_range.min) + ":" + std::to_string(process_config.cpu_time_range.max),
      io_block_period_table
    });

    processes_.push_back(process_config);
  }

  std::cout << table << std::endl;

  std::cout << "Initialization was successful!" << std::endl;
}

} // namespace os_lab2
