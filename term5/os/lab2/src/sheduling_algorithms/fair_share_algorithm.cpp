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

  if (process_config.io_block_period_range.has_value()) {
    io_block_period = { *process_config.io_block_period_range };
  }

  if (process_config.io_block_time_range.has_value()) {
    io_block_time = { *process_config.io_block_time_range };
  }
}

FairShareAlgorithm::FairShareAlgorithm()
  : user_count_(0)
  , process_count_(0) {

  table_.add_row({ 
    "name", 
    "status", 
    "user_id", 
    "cpu_time", 
    "io_block_period", 
    "io_block_time",
    "block_count",
    "taken_time",
    "priority"
  });

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
    user_configs_[process_config.user_id].process_count++;
  }

  user_count_     = user_configs_.size();
  process_count_  = config.process_configs.size();

  // Init processes
  processes_[Process::Status::kReady].reserve(config.process_configs.size());

  for (const auto& process_config : config.process_configs) {
    auto& process = processes_[Process::Status::kReady].emplace_back(
      std::make_shared<Process>(process_config, kBasePriority)
    );

    process->status = Process::Status::kReady;

    process->cpu_time.max = ng::random::Next<std::size_t>(
      process->cpu_time.range.min,
      process->cpu_time.range.max
    );

    if (process->io_block_period) {
      process->io_block_period->max = ng::random::Next<std::size_t>(
        process->io_block_period->range.min,
        process->io_block_period->range.max
      );
    }

    if (process->io_block_time) {
      process->io_block_time->max = ng::random::Next<std::size_t>(
        process->io_block_time->range.min,
        process->io_block_time->range.max
      );
    }
  }

  // Main loop
  while (result_.taken_time < config.simulation_time) {
    result_.taken_time += kContextSwitchTime;

    auto current_process_iterator = std::min_element(
      processes_[Process::Status::kReady].begin(), 
      processes_[Process::Status::kReady].end(), 
      [](const auto& lhs, const auto& rhs) {
        return lhs->priority < rhs->priority;
      }
    );

    auto current_process = *current_process_iterator;

    current_process->status = Process::Status::kRunning;

    // Log current process
    std::cout << current_process->name << ": " << "user_id = " << current_process->user_id << std::endl;

    PushRecord(*current_process, "before quantum");

    // Quantum simulation
    for (std::size_t tick = 0; tick < kQuantum; ++tick) {
      result_.taken_time++;

      // Update current process
      current_process->cpu_time.current++;
      current_process->cpu_count++;
      current_process->cpu_time_done++;
      user_configs_[current_process->user_id].cpu_count++;
      user_configs_[current_process->user_id].cpu_time_done++;

      auto set_new_current_process = [&] {
        current_process_iterator = std::min_element(
          processes_[Process::Status::kReady].begin(),
          processes_[Process::Status::kReady].end(),
          [&](const auto& lhs, const auto& rhs) {
            return lhs->priority < rhs->priority && lhs->user_id == current_process->user_id;
          }
        );

        current_process = *current_process_iterator;

        result_.taken_time += kContextSwitchTime;

        PushRecord(*current_process, "in quantum");
        current_process->status = Process::Status::kRunning;
        PushRecord(*current_process, "in quantum");
      };

      // If current_process is done
      if (current_process->cpu_time.current == current_process->cpu_time.max) {
        current_process->status = Process::Status::kDone;
        PushRecord(*current_process, "done!");

        user_configs_[current_process->user_id].process_count--;

        if (user_configs_[current_process->user_id].process_count == 0) {
          user_configs_.erase(current_process->user_id);
        }

        processes_[Process::Status::kReady].erase(current_process_iterator);

        if (tick + 2 < kQuantum) {
          set_new_current_process();
        }
      }

      // if current_process is blocked
      if (current_process->io_block_period) {
        current_process->io_block_period->current++;

        if (current_process->io_block_period->current == current_process->io_block_period->max) {
          current_process->status = Process::Status::kBlocked;
          PushRecord(*current_process, "blocked");

          current_process->block_count++;
          current_process->io_block_period->current  = 0;
          current_process->io_block_time->current    = 0;

          current_process->io_block_period->max = ng::random::Next<std::size_t>(
            current_process->io_block_period->range.min,
            current_process->io_block_period->range.max
          );

          current_process->io_block_time->max = ng::random::Next<std::size_t>(
            current_process->io_block_time->range.min,
            current_process->io_block_time->range.max
          );

          processes_[Process::Status::kBlocked].push_back(current_process);
          processes_[Process::Status::kReady].erase(current_process_iterator);

          if (tick + 2 < kQuantum) {
            set_new_current_process();
          }
        }
      }

      // Update blocked processes
      for (std::size_t i = 0; i < processes_[Process::Status::kBlocked].size();) {
        auto& blocked_process = processes_[Process::Status::kBlocked][i];

        blocked_process->io_block_time->current++;

        if (blocked_process->io_block_time->current == blocked_process->io_block_time->max) {
          blocked_process->status = Process::Status::kReady;
          PushRecord(*blocked_process, "from blocked to ready");

          processes_[Process::Status::kReady].push_back(blocked_process);
          processes_[Process::Status::kBlocked].erase(
            processes_[Process::Status::kBlocked].begin() + static_cast<ProcessPointerVector::iterator::difference_type>(i)
          );

          continue;
        }

        i++;
      }
    }

    if (current_process->status == Process::Status::kRunning) {
      current_process->status = Process::Status::kReady;
    }

    PushRecord(*current_process, "after quantum");

    if (user_configs_.empty()) {
      break;
    }

    // Update process priorities
    auto user_count = user_configs_.size();

    for (auto& [_, user_config] : user_configs_) {
      user_config.cpu_count /= user_count;
    }

    for (auto& process : processes_[Process::Status::kReady]) {
      process->priority = process->cpu_count / user_count + user_configs_[process->user_id].cpu_count / user_count + kBasePriority;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(60));
  }

  std::ofstream out_file(config.process_results_filepath);

  LogTable(std::cout);
  LogTable(out_file);

  out_file.close();

  if (user_configs_.size() == user_count_) {
    tabulate::Table user_config_table;

    user_config_table.add_row({ "user_id", "cpu_time_done" });

    for (const auto& [user_id, user_config] : user_configs_) {
      user_config_table.add_row({
        std::to_string(user_id),
        std::to_string(user_config.cpu_time_done)
      });
    }

    std::cout << user_config_table << std::endl;
  }

  std::size_t process_count = std::accumulate(
    processes_.begin(),
    processes_.end(),
    0,
    [](std::size_t sum, const auto& processes) {
      return sum + processes.second.size();
    }
  );

  if (process_count == process_count_) {
    tabulate::Table process_table;

    process_table.add_row({ "name", "cpu_time" });

    for (const auto& [status, processes] : processes_) {
      for (const auto& process : processes) {
        process_table.add_row({
          process->name,
          std::to_string(process->cpu_time_done)
        });
      }
    }

    std::cout << process_table << std::endl;
  }

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

  table_.column(1).format().font_align(tabulate::FontAlign::left);

  ostream << table_ << std::endl;
}

void FairShareAlgorithm::PushRecord(const Process& current_process, const std::string& comment) {
  auto status_to_string = [](Process::Status status) {
    std::string result;

    switch (status) {
      case Process::Status::kUnknown: result = "unknown"; break;
      case Process::Status::kReady:   result = "ready";   break;
      case Process::Status::kBlocked: result = "blocked"; break;
      case Process::Status::kPaused:  result = "paused";  break;
      case Process::Status::kDone:    result = "done";    break;
      case Process::Status::kRunning: result = "running"; break;
    }

    return result;
  };

  auto row_count = *reinterpret_cast<std::size_t*>(&table_);

  std::string io_block_period = "-";

  if (current_process.io_block_period) {
    io_block_period  = std::to_string(current_process.io_block_period->current) + ":";
    io_block_period += std::to_string(current_process.io_block_period->max);
  }

  std::string io_block_time = "-";

  if (current_process.io_block_time) {
    io_block_time  = std::to_string(current_process.io_block_time->current) + ":";
    io_block_time += std::to_string(current_process.io_block_time->max);
  }

  table_.add_row({
    current_process.name,
    status_to_string(current_process.status) + " -> " + comment,
    std::to_string(current_process.user_id),
    std::to_string(current_process.cpu_time.current) + ":" + std::to_string(current_process.cpu_time.max),
    io_block_period,
    io_block_time,
    std::to_string(current_process.block_count),
    std::to_string(result_.taken_time),
    std::to_string(current_process.priority)
  });

  tabulate::Color color = tabulate::Color::white;

  switch (current_process.status) {
    case Process::Status::kRunning: color = tabulate::Color::cyan;    break;
    case Process::Status::kReady:   color = tabulate::Color::yellow;  break;
    case Process::Status::kPaused:  color = tabulate::Color::white;   break;
    case Process::Status::kBlocked: color = tabulate::Color::red;     break;
    case Process::Status::kDone:    color = tabulate::Color::green;   break;
    default:                                                          break;
  }

  table_.row(row_count).format()
    .font_color(color);
}

} // namespace os_lab2
