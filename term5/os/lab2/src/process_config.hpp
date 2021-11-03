#pragma once

#include <cstdint>
#include <sstream>

namespace os_lab2 {

struct ProcessConfig {
  std::size_t cpu_time      = 0;
  std::size_t io_blocking   = 0;
  std::size_t cpu_done      = 0;
  std::size_t io_next       = 0;
  std::size_t blocked_count = 0;
};

} // namespace os_lab2
