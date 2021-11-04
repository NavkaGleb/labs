#pragma once

#include <cstdint>

namespace scheduler_simulator {

class CommandLineArgs {
 public:
  using ConstIterator = char**;

 public:
  CommandLineArgs(int argc, char** argv);

 public:
  [[nodiscard]] std::size_t GetCount() const;

 public:
  [[nodiscard]] ConstIterator Begin() const;
  [[nodiscard]] ConstIterator End() const;

 public:
  const char* operator [](std::size_t index) const;

 private:
  int     argc_;
  char**  argv_;
};

inline auto begin(const CommandLineArgs& args) {
  return args.Begin();
}

inline auto end(const CommandLineArgs& args) {
  return args.End();
}

} // namespace scheduler_simulator
