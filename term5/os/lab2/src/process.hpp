#pragma once

#include <cstdint>
#include <sstream>

namespace os_lab2 {

struct Process {
  std::size_t cpu_time      = 0;
  std::size_t io_blocking   = 0;
  std::size_t cpu_done      = 0;
  std::size_t io_next       = 0;
  std::size_t blocked_count = 0;

  std::string ToString() const {
    std::stringstream ss;

    ss << "cpu_time: " << cpu_time << "\t";
    ss << "io_blocking: " << io_blocking << "\t";
    ss << "cpu_done: " << cpu_done << "\t";
    ss << "io_next: " << io_next << "\t";
    ss << "blocked_count: " << blocked_count << "\t";

    return ss.str();
  }
};

} // namespace os_lab2
