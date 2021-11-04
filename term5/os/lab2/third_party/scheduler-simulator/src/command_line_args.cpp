#include "command_line_args.hpp"

namespace scheduler_simulator {

CommandLineArgs::CommandLineArgs(int argc, char** argv)
  : argc_(argc)
  , argv_(argv) {}

std::size_t CommandLineArgs::GetCount() const {
  return argc_;
}

CommandLineArgs::ConstIterator CommandLineArgs::Begin() const {
  return argv_;
}

CommandLineArgs::ConstIterator CommandLineArgs::End() const {
  return argv_ + argc_;
}

const char* CommandLineArgs::operator [](std::size_t index) const {
  return argv_[index];
}

} // namespace scheduler_simulator
