#include "first_come_first_served_algorithm.hpp"

#include <tabulate/table.hpp>

namespace os_lab2 {

SchedulingAlgorithm::Result FirstComeFirstServedAlgorithm::operator ()(
  std::size_t                 simulation_time,
  std::vector<ProcessConfig>& processes
) {
  tabulate::Table table;

  table.add_row({ "id", "comment", "cpu_time", "io_blocking", "cpu_done" });

  Result result;

  result.algorithm_type = "Batch (NonPreemptive)";
  result.algorithm_name = "First-Come First-Served";

  std::size_t current_process_index = 0;
  std::size_t previous_process_index = 0;

  std::size_t completed = 0;

  std::size_t current_row = 0;

  auto add_row = [&](const std::string& comment) {
    table.add_row({
      std::to_string(current_process_index),
      comment,
      std::to_string(processes[current_process_index].cpu_time),
      std::to_string(processes[current_process_index].io_blocking),
      std::to_string(processes[current_process_index].cpu_done)
    }).row(current_row + 1).format().hide_border_bottom().hide_border_top();

    current_row++;
  };

  add_row("registered...");

  while (result.taken_time < simulation_time) {
    if (processes[current_process_index].cpu_done == processes[current_process_index].cpu_time) {
      completed++;

      add_row("completed...");
      table[current_row].format().font_color(tabulate::Color::green);

      if (completed == processes.size()) {
        break;
      }

      for (std::size_t i = processes.size() - 1; i + 1 > 0; --i) {
        if (processes[i].cpu_done < processes[i].cpu_time) {
          current_process_index = i;
        }
      }

      add_row("registered...");
    }

    if (processes[current_process_index].io_blocking == processes[current_process_index].io_next) {
      add_row("io blocked...");
      table.row(current_row).format().font_color(tabulate::Color::red);

      processes[current_process_index].blocked_count++;
      processes[current_process_index].io_next = 0;

      previous_process_index = current_process_index;

      for (std::size_t i = processes.size() - 1; i + 1 > 0; --i) {
        if (processes[i].cpu_done < processes[i].cpu_time && previous_process_index != i) {
          current_process_index = i;
        }
      }

      add_row("registered...");
    }

    processes[current_process_index].cpu_done++;

    if (processes[current_process_index].io_blocking > 0) {
      processes[current_process_index].io_next++;
    }

    result.taken_time++;
  }

  table.row(current_row).format().show_border_bottom();

  std::cout << table << std::endl;

  return result;
}

} // namespace os_lab2
