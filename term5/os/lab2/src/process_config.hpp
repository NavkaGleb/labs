#pragma once

#include <cstdint>
#include <sstream>

namespace os_lab2 {

struct ProcessConfig {
  std::string name;
  std::size_t max_cpu_time = 0;
  std::size_t io_blocking  = 0;
  std::size_t block_count  = 0;
  std::size_t user_id      = 0;
};

} // namespace os_lab2
