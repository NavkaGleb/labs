#include "first_come_first_served_algorithm.hpp"

#include <fstream>

#include <tabulate/table.hpp>

namespace os_lab2 {

SchedulingAlgorithm::Result FirstComeFirstServedAlgorithm::operator ()(
  std::size_t                  simulation_time,
  const std::vector<ProcessConfig>&  processes
) {
//  tabulate::Table table;
//  std::size_t     record_count = 0;
//
//  table.add_row({ "id", "comment", "cpu_time", "io_blocking", "cpu_done" });
//
  Result result = { "Batch (NonPreemptive)", "First-Come First-Served" };
//
//  std::size_t current_process_index = 0;
//  std::size_t previous_process_index = 0;
//
//  std::size_t completed = 0;
//
//  auto add_row = [&](const std::string& comment, tabulate::Color font_color = tabulate::Color::white) {
//    table.add_row({
//        std::to_string(current_process_index),
//        comment,
//        std::to_string(processes[current_process_index].max_cpu_time),
//        std::to_string(processes[current_process_index].io_blocking),
////        std::to_string(processes[current_process_index].curr)
//    });
//
//    if (record_count != 0) {
//      table.row(record_count + 1).format()
//          .hide_border_top()
//          .hide_border_bottom()
//          .font_color(font_color);
//    }
//
//    record_count++;
//  };
//
//  add_row("registered...");
//
//  while (result.taken_time < simulation_time) {
//    if (processes[current_process_index].cpu_done == processes[current_process_index].cpu_time) {
//      completed++;
//
//      add_row("completed...", tabulate::Color::green);
//
//      if (completed == processes.size()) {
//        break;
//      }
//
//      for (std::size_t i = processes.size() - 1; i + 1 > 0; --i) {
//        if (processes[i].cpu_done < processes[i].cpu_time) {
//          current_process_index = i;
//        }
//      }
//
//      add_row("registered...");
//    }
//
//    if (processes[current_process_index].io_blocking == processes[current_process_index].io_next) {
//      add_row("io blocked...", tabulate::Color::red);
//
//      processes[current_process_index].block_count++;
//      processes[current_process_index].io_next = 0;
//
//      previous_process_index = current_process_index;
//
//      for (std::size_t i = processes.size() - 1; i + 1 > 0; --i) {
//        if (processes[i].cpu_done < processes[i].cpu_time && previous_process_index != i) {
//          current_process_index = i;
//        }
//      }
//
//      add_row("registered...");
//    }
//
//    processes[current_process_index].cpu_done++;
//
//    if (processes[current_process_index].io_blocking > 0) {
//      processes[current_process_index].io_next++;
//    }
//
//    result.taken_time++;
//  }
//
//  table.row(record_count).format().show_border_bottom();
//
//
//  std::ofstream out_file("assets/process_result.txt");
//
//  std::cout << table << std::endl;
//  out_file << table << std::endl;
//
//  out_file.close();
//
  return result;
}

} // namespace os_lab2
