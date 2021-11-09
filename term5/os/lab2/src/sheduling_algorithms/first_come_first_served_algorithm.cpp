#include "first_come_first_served_algorithm.hpp"

#include <fstream>

#include <tabulate/table.hpp>

#include <ng_random/random.hpp>

namespace os_lab2 {

struct Process : ProcessConfig {
  std::size_t                       current_cpu_time  = 0;
  std::size_t                       io_block_next     = 0;
  std::size_t                       block_count       = 0;
  std::size_t                       max_cpu_time      = 0;
  std::optional<Range<std::size_t>> io_block_range;

  explicit Process(const ProcessConfig& process_config) {
    name            = process_config.name;
    user_id         = process_config.user_id;

    max_cpu_time    = ng::random::Next<std::size_t>(
      process_config.cpu_time_range.min,
      process_config.cpu_time_range.max
    );

    io_block_range = process_config.io_block_range;
  }
};

SchedulingAlgorithm::Result FirstComeFirstServedAlgorithm::operator ()(Config&& config) {
  tabulate::Table table;

  table.add_row({ "id", "comment", "cpu_time", "io_block_period", "block_count" });

  Result result = { "Batch (NonPreemptive)", "First-Come First-Served" };

  std::size_t current_process_index = 0;
  std::size_t previous_process_index = 0;
  std::size_t completed = 0;

  std::vector<Process> processes;

  processes.reserve(config.process_configs.size());

  for (const auto& process_config : config.process_configs) {
    processes.emplace_back(process_config);
  }

  auto add_row = [&](const std::string& comment, tabulate::Color font_color = tabulate::Color::white) {
    tabulate::Table cpu_time_table;

    cpu_time_table.add_row({
      std::to_string(processes[current_process_index].max_cpu_time),
      std::to_string(processes[current_process_index].current_cpu_time)
    }).format().hide_border();

    bool is_io_block_period_exists = processes[current_process_index].io_block_range.has_value();

    table.add_row({
        std::to_string(current_process_index),
        comment,
        cpu_time_table,
        is_io_block_period_exists ? std::to_string(processes[current_process_index].io_block_range->max) : "-",
        std::to_string(processes[current_process_index].block_count)
    });

    std::size_t current_row = *reinterpret_cast<std::size_t*>(&table) - 1;

    if (current_row != 0) {
      table.row(current_row).format()
        .hide_border_top()
        .hide_border_bottom()
        .font_color(font_color);
    }
  };

  add_row("registered...");

  while (result.taken_time < config.simulation_time) {
    if (processes[current_process_index].current_cpu_time == processes[current_process_index].max_cpu_time) {
      completed++;

      add_row("completed...", tabulate::Color::green);

      if (completed == processes.size()) {
        break;
      }

      for (std::size_t i = processes.size() - 1; i + 1 > 0; --i) {
        if (processes[i].current_cpu_time < processes[i].max_cpu_time) {
          current_process_index = i;
        }
      }

      add_row("registered...");
    }

    if (processes[current_process_index].io_block_range) {
      if (processes[current_process_index].io_block_range->max == processes[current_process_index].io_block_next) {
        add_row("io blocked...", tabulate::Color::red);

        processes[current_process_index].block_count++;
        processes[current_process_index].io_block_next = 0;

        previous_process_index = current_process_index;

        for (std::size_t i = processes.size() - 1; i + 1 > 0; --i) {
          if (processes[i].current_cpu_time < processes[i].max_cpu_time && previous_process_index != i) {
            current_process_index = i;
          }
        }

        add_row("registered...");
      }
    }

    processes[current_process_index].current_cpu_time++;

    if (processes[current_process_index].io_block_range) {
      processes[current_process_index].io_block_next++;
    }

    result.taken_time++;
  }

  std::size_t current_row = *reinterpret_cast<std::size_t*>(&table) - 1;

  table.row(current_row).format().show_border_bottom();

  std::ofstream out_file(config.process_results_filepath);

  std::cout << table << std::endl;
  out_file << table << std::endl;

  out_file.close();

  return result;
}

} // namespace os_lab2
