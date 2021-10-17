#ifndef OS_LAB1_MANAGER_HPP_
#define OS_LAB1_MANAGER_HPP_

#include <termios.h>

#include "process.hpp"
#include "keyboard.hpp"
#include "computation_function.hpp"
#include "nonblocking_read_poll.hpp"
#include "unnamed_pipe.hpp"

namespace os_lab1 {

template <typename T>
class Manager {
 private:
  enum class UserResponse : std::uint8_t {
    kYes = 0,
    kNo,
    kTimeout
  };

 private:
  struct ComputationFunctionContainer {
    ProcessId                     process_id;
    ComputationFunction<T>        instance;
    ComputationFunctionResult<T>  result;
    UnnamedPipe                   pipe;
  };

 public:
  Manager(const ComputationFunction<T>& f, const ComputationFunction<T>& g);

 public:
  ~Manager();

 public:
  void Run();

 private:
  [[nodiscard]] NonblockingReadPoll::PipeListenCallback GetPollPipeListenCallback();
  [[nodiscard]] NonblockingReadPoll::ListenCallback GetPollListenCallback();

  UserResponse Confirm(const std::string& message, std::uint32_t seconds);
  void Exit();

 private:
  const std::size_t kSoftFailAttemptCount = 4;

 private:
  Keyboard                      keyboard_;
  NonblockingReadPoll           poll_;
  ComputationFunctionContainer  f_;
  ComputationFunctionContainer  g_;

  bool                          is_running_;
};

} // namespace os_lab1

#include "manager.inl"

#endif // OS_LAB1_MANAGER_HPP_
