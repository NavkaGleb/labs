#include "process.hpp"

#include <cassert>
#include <iostream>

#include <unistd.h>
#include <sys/wait.h>

namespace os_lab1 {

std::size_t Process::child_process_count_ = 0;

Process Process::GetCurrent() {
  return { getpid(), getppid() };
}

Process::Process()
  : id_(-1)
  , parent_id_(-1) {}

Process::Process(ProcessId id, ProcessId parent_id)
  : id_(id)
  , parent_id_(parent_id) {}

ProcessId Process::GetId() const {
  return id_;
}

ProcessId Process::GetParentId() const {
  return parent_id_;
}

bool Process::IsChild(const Process& child) const {
  return id_ == child.parent_id_;
}

Process Process::SpanChild() const {
  if (child_process_count_ == 0 || Process::GetCurrent().IsChild(*this)) {
    Process child;

    auto process_id = fork();

    assert(process_id != -1 && "Failed to create process!");

    if (process_id == 0) {
      child.id_         = getpid();
      child.parent_id_  = getppid();

      child_process_count_++;
    }

    return child;
  }

  return {};
}

void Process::WaitForChild() const {
  auto status = wait(nullptr);

  assert(status != 1 && "Failed to wait for child!");
}

void Process::WaitForChildren() const {
  while (auto status = wait(nullptr)) {
    assert(status != -1 && "Failed to wait for children!");
  }
}

Process::operator bool() const {
  return id_ != -1 && parent_id_ != -1;
}

} // namespace os_lab1
