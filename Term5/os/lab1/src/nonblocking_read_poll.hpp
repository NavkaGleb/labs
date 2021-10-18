#ifndef OS_LAB1_SRC_POLL_HPP_
#define OS_LAB1_SRC_POLL_HPP_

#include <vector>
#include <functional>

#include <poll.h>

#include "unnamed_pipe.hpp"

namespace os_lab1 {

class NonblockingReadPoll {
 public:
  using PipeListenCallback  = std::function<void(const UnnamedPipe& pipe)>;
  using ListenCallback      = std::function<void()>;

 public:
  struct Config {
    std::vector<UnnamedPipe>  pipes;
    PipeListenCallback        pipe_listen_callback;
    ListenCallback            listen_callback;
  };

 public:
  NonblockingReadPoll();
  explicit NonblockingReadPoll(Config&& config);

 public:
  void Listen();
  void StopListen();

 private:
  std::vector<UnnamedPipe>  pipes_;
  std::vector<pollfd>       file_descriptors_;
  PipeListenCallback        pipe_listen_callback_;
  ListenCallback            listen_callback_;
  bool                      is_listening_;
};

} // namespace os_lab1

#endif // OS_LAB1_SRC_POLL_HPP_
