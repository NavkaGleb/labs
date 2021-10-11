#ifndef OS_LAB1_SRC_PROCESS_HPP_
#define OS_LAB1_SRC_PROCESS_HPP_

#include <cstdint>

#include <unistd.h>

namespace os_lab1 {

using ProcessId = pid_t;

class Process {
 public:
  static Process GetCurrent();

 public:
  Process();

 private:
  Process(ProcessId id, ProcessId parent_id);

 public:
  [[nodiscard]] ProcessId GetId() const;
  [[nodiscard]] ProcessId GetParentId() const;

 public:
  [[nodiscard]] bool IsChild(const Process& child) const;

  [[nodiscard]] Process SpanChild() const;

  void WaitForChild() const;
  void WaitForChildren() const;

 public:
  explicit operator bool() const;

 private:
  static std::size_t child_process_count_;

 private:
  ProcessId id_;
  ProcessId parent_id_;
};

} // namespace os_lab1

#endif // OS_LAB1_SRC_PROCESS_HPP_
