#pragma once

#include <cstdint>
#include <sstream>

namespace os_lab2 {

template <typename T>
struct Range {
  T average = T();
  T deviation = T();
};

struct ProcessConfig {
  std::string         name;

  std::size_t         user_id = 0;
  std::size_t         max_cpu_time = 0;

  Range<std::size_t>  io_block_time;
  Range<std::size_t>  io_block_period;
};

} // namespace os_lab2
