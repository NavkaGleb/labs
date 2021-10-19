#include "process.hpp"

#include <stdexcept>
#include <cerrno>
#include <cstring>

#include <unistd.h>
#include <sys/wait.h>

namespace os_lab1 {

namespace process {

void Kill(ProcessId id, int signal) {
  kill(id, signal);
}

} // namespace process

namespace this_process {

ProcessId GetId() noexcept {
  return getpid();
}

ProcessId GetParentId() noexcept {
  return getppid();
}

ProcessId SpawnChild(const ProcessExecutor &process) {
  auto process_id = fork();

  if (process_id == -1) {
    throw std::runtime_error(strerror(errno));
  }

  if (process_id == 0) {
    std::exit(process());
  }

  return process_id;
}

ProcessId SplitExecution(const ProcessExecutor &process) {
  auto process_id = fork();

  if (process_id == -1) {
    throw std::runtime_error(strerror(errno));
  }

  if (process_id == 0) {
    process();
  }

  return process_id;
}

void WaitForChild() {
  auto process_id = wait(nullptr);

  if (process_id == -1) {
    throw std::runtime_error(strerror(errno));
  }
}

void WaitForChildren() {
  while (auto process_id = wait(nullptr)) {
    if (process_id == -1) {
      throw std::runtime_error(strerror(errno));
    }
  }
}

void Kill(int status) noexcept {
  std::exit(status);
}

} // namespace this_process

} // namespace os_lab1
