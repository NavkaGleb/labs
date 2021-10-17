#include "process.hpp"

#include <cassert>
#include <iostream>

#include <unistd.h>
#include <sys/wait.h>

namespace os_lab1 {

namespace process {

void Kill(ProcessId id, int signal) {
  kill(id, signal);
}

} // namespace process

namespace this_process {

ProcessId GetId() {
  return getpid();
}

ProcessId GetParentId() {
  return getppid();
}

ProcessId SpawnChild(const ProcessExecutor &process) {
  auto process_id = fork();

  assert(process_id != -1);

  if (process_id == 0) {
    std::exit(process());
  }

  return process_id;
}

ProcessId SplitExecution(const ProcessExecutor &process) {
  auto process_id = fork();

  assert(process_id != -1);

  if (process_id == 0) {
    process();
  }

  return process_id;
}

void WaitForChild() {
  int status;

  wait(&status);

  assert(status != -1);
}

void WaitForChildren() {
  while (auto process_id = wait(nullptr)) {
    assert(process_id != -1);
  }
}

void Kill(int status) {
  std::exit(status);
}

} // namespace this_process

} // namespace os_lab1
