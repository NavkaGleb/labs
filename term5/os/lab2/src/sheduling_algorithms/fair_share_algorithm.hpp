#pragma once

#include <vector>
#include <map>
#include <memory>

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
      kReady,
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
    std::optional<Limit<std::size_t>> io_block_period;
    std::optional<Limit<std::size_t>> io_block_time;
    Limit<std::size_t>                cpu_time;
    std::size_t                       cpu_time_done = 0;

    explicit Process(const ProcessConfig& process_config, std::size_t priority);
  };

  struct UserConfig {
    std::size_t cpu_count     = 0;
    std::size_t process_count = 0;
    std::size_t cpu_time_done = 0;
  };

 private:
  using ProcessPointer        = std::shared_ptr<Process>;
  using ProcessPointerVector  = std::vector<ProcessPointer>;
  using ProcessContainer      = std::map<Process::Status, ProcessPointerVector>;
  using UserConfigContainer   = std::map<std::size_t, UserConfig>;

 private:
  void LogTable(std::ostream& ostream);
  void PushRecord(const Process& current_process, const std::string& comment);

 private:
  static inline const std::size_t kContextSwitchTime = 1;
  static inline const std::size_t kQuantum           = 50;
  static inline const std::size_t kBasePriority      = 50;

 private:
  tabulate::Table     table_;
  ProcessContainer    processes_;
  UserConfigContainer user_configs_;
  Result              result_;
  std::size_t         user_count_;
  std::size_t         process_count_;
};

} // namespace os_lab2
