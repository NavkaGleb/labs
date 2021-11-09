#pragma once

#include <cstdint>
#include <string>
#include <optional>

namespace os_lab2 {

template <typename T>
struct Range {
  T min = T();
  T max = T();
};

struct ProcessConfig {
  std::string                       name;
  std::size_t                       user_id = 0;
  Range<std::size_t>                cpu_time_range;
  std::optional<Range<std::size_t>> io_block_range;
};

} // namespace os_lab2
