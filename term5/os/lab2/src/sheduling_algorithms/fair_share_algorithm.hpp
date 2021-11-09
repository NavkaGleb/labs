#pragma once

#include <vector>
#include <map>

#include "../scheduling_algorithm.hpp"

#include <tabulate/table.hpp>

namespace os_lab2 {

class FairShareAlgorithm : public SchedulingAlgorithm {
 public:
  FairShareAlgorithm();

 public:
  Result operator ()(
    std::size_t                       simulation_time,
    const std::vector<ProcessConfig>& process_configs
  ) override;

 private:
  struct Process {
    std::string name;
    std::size_t priority          = 0;
    std::size_t max_cpu_time      = 0;
    std::size_t current_cpu_time  = 0;
    std::size_t user_id           = 0;
    std::size_t block_count       = 0;
    std::size_t cpu_count         = 0;
  };

  struct UserConfig {
    std::size_t cpu_count     = 0;
    std::size_t process_count = 0;
  };

 private:
  [[nodiscard]] bool IsDone() const;

  void InitUserConfigs(const std::vector<ProcessConfig>& process_configs);
  void InitProcesses(const std::vector<ProcessConfig>& process_configs);

  Process& GetCurrentProcess();

  void UpdateCurrentProcess(Process& current_process);
  void UpdateProcessesPriority();

  void LogCurrentProcess(const Process& current_process);
  void LogTable();
  void PushRecord(const Process& current_process, const std::string& comment, tabulate::Color color);

 private:
  static const std::size_t kQuantum = 60;
  static const std::size_t kBasePriority = 60;

 private:
  tabulate::Table table_;
  std::vector<Process> processes_;
  std::map<std::size_t, UserConfig>  user_configs;
  Result result_;
};

} // namespace os_lab2
