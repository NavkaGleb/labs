#pragma once

#include <cstdint>
#include <sstream>

namespace scheduler_simulator {

struct ProcessConfig {
  std::size_t cpu_time    = 0;
  std::size_t io_blocking = 0;
  std::size_t cpu_done    = 0;
  std::size_t io_next     = 0;
  std::size_t block_count = 0;
};

} // namespace scheduler_simulator
