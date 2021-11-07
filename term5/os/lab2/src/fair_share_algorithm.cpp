#include "fair_share_algorithm.hpp"

#include <iostream>
#include <thread>
#include <chrono>

namespace os_lab2 {

FairShareAlgorithm::FairShareAlgorithm() {
  table_.add_row({ "name", "comment", "max_cpu_time", "current_cpu_time", "user_id", "block_count" });

  result_.algorithm_type = "Scheduling in Interactive Systems";
  result_.algorithm_name = "Fair-Share Algorithm";
  result_.taken_time = 0;
}

scheduler_simulator::SchedulingAlgorithm::Result FairShareAlgorithm::operator ()(
  std::size_t                                             simulation_time,
  const std::vector<scheduler_simulator::ProcessConfig>&  process_configs
) {
  InitUserConfigs(process_configs);
  InitProcesses(process_configs);

  while (result_.taken_time < simulation_time) {
    auto& current_process = GetCurrentProcess();

    LogCurrentProcess(current_process);
    PushRecord(current_process, "registered...", tabulate::Color::white);

    UpdateCurrentProcess(current_process);

    if (IsDone()) {
      break;
    }

    UpdateProcessesPriority();
  }

  LogTable();

  return result_;
}

bool FairShareAlgorithm::IsDone() const {
  return user_configs.empty();
}

void FairShareAlgorithm::InitUserConfigs(const std::vector<scheduler_simulator::ProcessConfig>& process_configs) {
  for (const auto& process_config : process_configs) {
    user_configs[process_config.user_id].process_count++;
  }
}

void FairShareAlgorithm::InitProcesses(const std::vector<scheduler_simulator::ProcessConfig>& process_configs) {
  for (const auto& process_config : process_configs) {
    Process process;

    process.priority      = kBasePriority;
    process.name          = process_config.name;
    process.max_cpu_time  = process_config.max_cpu_time;
    process.user_id       = process_config.user_id;

    processes_.push_back(process);
  }
}

FairShareAlgorithm::Process& FairShareAlgorithm::GetCurrentProcess() {
  return *std::min_element(processes_.begin(), processes_.end(), [](const Process& lhs, const Process& rhs) {
    return lhs.priority < rhs.priority;
  });
}

void FairShareAlgorithm::UpdateCurrentProcess(Process& current_process) {
  result_.taken_time += kQuantum;

  current_process.current_cpu_time += kQuantum;
  current_process.cpu_count += kQuantum;

  user_configs[current_process.user_id].cpu_count += kQuantum;

  if (current_process.current_cpu_time >= current_process.max_cpu_time) {
    current_process.current_cpu_time = current_process.max_cpu_time;

    PushRecord(current_process, "done!", tabulate::Color::green);

    user_configs[current_process.user_id].process_count--;
    result_.taken_time += kQuantum;

    if (user_configs[current_process.user_id].process_count == 0) {
      user_configs.erase(current_process.user_id);
    }

    auto new_end = std::remove_if(processes_.begin(), processes_.end(), [&](const Process& process) {
      return process.name == current_process.name;
    });

    processes_.erase(new_end, processes_.end());
  }

  if (user_configs.size() != 1) {
    current_process.block_count++;
  }
}

void FairShareAlgorithm::UpdateProcessesPriority() {
  auto user_count = user_configs.size();

  for (auto& [_, user_config] : user_configs) {
    user_config.cpu_count /= user_count;
  }

  for (auto& process : processes_) {
    process.cpu_count /= user_count;
    process.priority = process.cpu_count / user_count + user_configs[process.user_id].cpu_count / user_count + kBasePriority;
  }
}

void FairShareAlgorithm::LogCurrentProcess(const Process& current_process) {
  std::cout << current_process.name << ": ";
  std::cout << "user_id = " << current_process.user_id;
  std::cout << ", cpu_time = " << current_process.current_cpu_time << ":" << current_process.max_cpu_time << std::endl;
}

void FairShareAlgorithm::LogTable() {
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

  std::cout << table_ << std::endl;
}

void FairShareAlgorithm::PushRecord(const Process& current_process, const std::string& comment, tabulate::Color color) {
  auto row_count = *reinterpret_cast<std::size_t*>(&table_);

  table_.add_row({
    current_process.name,
    comment,
    std::to_string(current_process.max_cpu_time),
    std::to_string(current_process.current_cpu_time),
    std::to_string(current_process.user_id),
    std::to_string(current_process.block_count)
  });

  table_.row(row_count - 1).format()
    .font_color(color);
}

} // namespace os_lab2
