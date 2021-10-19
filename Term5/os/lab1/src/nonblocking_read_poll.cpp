#include "nonblocking_read_poll.hpp"

#include <stdexcept>

#include <cerrno>
#include <cstring>

namespace os_lab1 {

NonblockingReadPoll::NonblockingReadPoll()
  : is_listening_(true) {}

NonblockingReadPoll::NonblockingReadPoll(Config&& config)
  : pipes_(std::move(config.pipes))
  , pipe_listen_callback_(std::move(config.pipe_listen_callback))
  , listen_callback_(std::move(config.listen_callback))
  , is_listening_(true) {

  const auto listener_count = pipes_.size();

  file_descriptors_.resize(listener_count);

  for (size_t i = 0; i < listener_count; ++i) {
    file_descriptors_[i].fd     = pipes_[i].GetReadDescriptor();
    file_descriptors_[i].events = POLLIN;
  }
}

void NonblockingReadPoll::Listen() {
  if (file_descriptors_.empty()) {
    throw std::invalid_argument("Don't init size of listeners in NonblockingReadPoll");
  }

  if (!pipe_listen_callback_) {
    throw std::invalid_argument("Don't provide callback for listener");
  }

  while (is_listening_) {
    int descriptors_count = poll(file_descriptors_.data(), file_descriptors_.size(), 0);

    if (descriptors_count == -1) {
      throw std::runtime_error(strerror(errno));
    }

    if (listen_callback_) {
      listen_callback_();
    }

    if (descriptors_count == 0) {
      continue;
    }

    for (std::size_t i = 0; i < file_descriptors_.size(); ++i) {
      if (file_descriptors_[i].revents & POLLIN) {
        pipe_listen_callback_(pipes_[i]);
      }
    }
  }
}

void NonblockingReadPoll::StopListen() noexcept {
  is_listening_ = false;
}

} // namespace os_lab1