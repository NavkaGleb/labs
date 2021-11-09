#pragma once

#include <vector>
#include <map>

#include <tabulate/table.hpp>

#include "scheduling_algorithm.hpp"

namespace os_lab2 {

class FairShareAlgorithm : public SchedulingAlgorithm {
 public:
  FairShareAlgorithm();

 public:
  Result operator ()(Config&& config) override;

 private:
  struct Process {
    enum class Status : uint8_t {
      kUnknown = 0,
      kRunning,
      kPaused,
      kBlocked,
      kDone,
    };

    template <typename T>
    struct Limit {
      Range<T>  range;
      T         max     = T();
      T         current = T();
    };

    std::string                       name;
    std::size_t                       user_id       = 0;
    std::size_t                       priority      = 0;
    std::size_t                       block_count   = 0;
    std::size_t                       cpu_count     = 0;
    Status                            status        = Status::kUnknown;
    std::optional<Limit<std::size_t>> io_block_time;
    Limit<std::size_t>                cpu_time;

    explicit Process(const ProcessConfig& process_config, std::size_t priority);
  };

  struct UserConfig {
    std::size_t cpu_count     = 0;
    std::size_t process_count = 0;
  };

 private:
  void LogTable(std::ostream& ostream);
  void PushRecord(const Process& current_process);

 private:
  static inline const std::size_t kQuantum      = 60;
  static inline const std::size_t kBasePriority = 60;

 private:
  tabulate::Table                   table_;
  std::vector<Process>              processes_;
  std::map<std::size_t, UserConfig> user_configs;
  Result                            result_;
};

} // namespace os_lab2
