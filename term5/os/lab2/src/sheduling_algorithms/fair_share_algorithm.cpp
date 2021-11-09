#include "fair_share_algorithm.hpp"

#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

#include <ng_random/random.hpp>

namespace os_lab2 {

FairShareAlgorithm::Process::Process(const ProcessConfig &process_config, std::size_t priority)
  : priority(priority) {

  name            = process_config.name;
  user_id         = process_config.user_id;
  cpu_time.range  = process_config.cpu_time_range;

  if (process_config.io_block_range.has_value()) {
    io_block_time = { *process_config.io_block_range };
  }
}

FairShareAlgorithm::FairShareAlgorithm() {
  table_.add_row({ "name", "status", "user_id", "cpu_time", "io_block_time", "block_count", "taken_time" });

  table_.format()
    .font_align(tabulate::FontAlign::right);

  table_.row(0).format()
    .font_color(tabulate::Color::yellow)
    .font_style({ tabulate::FontStyle::bold });

  result_.algorithm_type  = "Scheduling in Interactive Systems";
  result_.algorithm_name  = "Fair-Share Algorithm";
  result_.taken_time      = 0;
}

SchedulingAlgorithm::Result FairShareAlgorithm::operator ()(Config&& config) {
  // Init user group
  for (const auto& process_config : config.process_configs) {
    user_configs[process_config.user_id].process_count++;
  }

  // Init processes
  processes_.reserve(config.process_configs.size());

  for (const auto& process_config : config.process_configs) {
    auto& process = processes_.emplace_back(process_config, kBasePriority);

    process.cpu_time.max = ng::random::Next<std::size_t>(
      process.cpu_time.range.min,
      process.cpu_time.range.max
    );

    process.io_block_time->max = ng::random::Next<std::size_t>(
      process.io_block_time->range.min,
      process.io_block_time->range.max
    );
  }

  // Main loop
  while (result_.taken_time < config.simulation_time) {
    result_.taken_time += kContextSwitchTime;

    auto current_process_iterator = std::min_element(processes_.begin(), processes_.end(), [](const Process& lhs, const Process& rhs) {
      return lhs.priority < rhs.priority;
    });

    auto& current_process = *current_process_iterator;

    current_process.status = Process::Status::kRunning;

    // Log current process
    std::cout << current_process.name << ": " << "user_id = " << current_process.user_id << std::endl;

    PushRecord(current_process);

    // Quantum simulation
    for (std::size_t tick = 0; tick < kQuantum; ++tick) {
      result_.taken_time++;

      // Update current process
      current_process.cpu_time.current++;
      current_process.cpu_count++;
      user_configs[current_process.user_id].cpu_count++;

      if (current_process.cpu_time.current == current_process.cpu_time.max) {
        current_process.status = Process::Status::kDone;
        break;
      }

      if (current_process.io_block_time) {
        current_process.io_block_time->current++;

        if (current_process.io_block_time->current == current_process.io_block_time->max) {
          current_process.status = Process::Status::kBlocked;
          break;
        }
      }
    }

    if (current_process.status == Process::Status::kRunning) {
      current_process.status = Process::Status::kPaused;
    }

    PushRecord(current_process);

    switch (current_process.status) {
      case Process::Status::kDone: {
        current_process.cpu_time.current = current_process.cpu_time.max;

        user_configs[current_process.user_id].process_count--;

        if (user_configs[current_process.user_id].process_count == 0) {
          user_configs.erase(current_process.user_id);
        }

        processes_.erase(current_process_iterator);
      }

      case Process::Status::kBlocked: {
        current_process.block_count++;
        current_process.io_block_time->current = 0;

        current_process.io_block_time->max = ng::random::Next<std::size_t>(
          current_process.io_block_time->range.min,
          current_process.io_block_time->range.max
        );
      }

      default: break;
    }

    if (user_configs.empty()) {
      break;
    }

    // Update process priorities
    auto user_count = user_configs.size();

    for (auto& [_, user_config] : user_configs) {
      user_config.cpu_count /= user_count;
    }

    for (auto& process : processes_) {
      process.cpu_count /= user_count;
      process.priority = process.cpu_count / user_count + user_configs[process.user_id].cpu_count / user_count + kBasePriority;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(60));
  }

  std::ofstream out_file(config.process_results_filepath);

  LogTable(std::cout);
  LogTable(out_file);

  out_file.close();

  return result_;
}

void FairShareAlgorithm::LogTable(std::ostream& ostream) {
  auto row_count = *reinterpret_cast<std::size_t*>(&table_);

  for (std::size_t i = 1; i < row_count; ++i) {
    auto& current_row = table_.row(i);

    if (i == 1) {
      continue;
    } else if (i == row_count - 1) {
      current_row.format().hide_border_top();
    } else {
      current_row.format()
        .hide_border_top()
        .hide_border_bottom();
    }
  }

  ostream << table_ << std::endl;
}

void FairShareAlgorithm::PushRecord(const Process& current_process) {
  auto status_to_string = [](Process::Status status) {
    std::string result;

    switch (status) {
      case Process::Status::kUnknown: result = "unknown"; break;
      case Process::Status::kBlocked: result = "blocked"; break;
      case Process::Status::kPaused:  result = "paused";  break;
      case Process::Status::kDone:    result = "done";    break;
      case Process::Status::kRunning: result = "running"; break;
    }

    return result;
  };

  auto row_count = *reinterpret_cast<std::size_t*>(&table_);

  std::string io_block_time = "-";

  if (current_process.io_block_time) {
    io_block_time  = std::to_string(current_process.io_block_time->current) + ":";
    io_block_time += std::to_string(current_process.io_block_time->max);
  }

  table_.add_row({
    current_process.name,
    status_to_string(current_process.status),
    std::to_string(current_process.user_id),
    std::to_string(current_process.cpu_time.current) + ":" + std::to_string(current_process.cpu_time.max),
    io_block_time,
    std::to_string(current_process.block_count),
    std::to_string(result_.taken_time)
  });

  tabulate::Color color = tabulate::Color::white;

  switch (current_process.status) {
    case Process::Status::kRunning: color = tabulate::Color::cyan;  break;
    case Process::Status::kPaused:  color = tabulate::Color::white; break;
    case Process::Status::kBlocked: color = tabulate::Color::red;   break;
    case Process::Status::kDone:    color = tabulate::Color::green; break;
    default:                                                        break;
  }

  table_.row(row_count).format()
    .font_color(color);
}

} // namespace os_lab2
