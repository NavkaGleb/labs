#include "nonblocking_read_poll.hpp"

namespace os_lab1 {

NonblockingReadPoll::NonblockingReadPoll()
  : is_listening_(false) {}

NonblockingReadPoll::NonblockingReadPoll(Config&& config)
  : pipes_(std::move(config.pipes))
  , pipe_listen_callback_(std::move(config.pipe_listen_callback))
  , listen_callback_(std::move(config.listen_callback))
  , is_listening_(false) {

  const auto listener_count = pipes_.size();

  file_descriptors_.resize(listener_count);

  for (size_t i = 0; i < listener_count; ++i) {
    file_descriptors_[i].fd     = pipes_[i].GetReadDescriptor();
    file_descriptors_[i].events = POLLIN;
  }
}

void NonblockingReadPoll::Listen() {
  assert(!file_descriptors_.empty());
  assert(pipe_listen_callback_);

  is_listening_ = true;

  while (is_listening_) {
    int descriptors_count = poll(file_descriptors_.data(), file_descriptors_.size(), 0);

    if (listen_callback_) {
      listen_callback_();
    }

    if (descriptors_count == 0) {
      continue;
    }

    assert(descriptors_count != -1);

    for (std::size_t i = 0; i < file_descriptors_.size(); ++i) {
      if (file_descriptors_[i].revents & POLLIN) {
        pipe_listen_callback_(pipes_[i]);
      }
    }
  }
}

void NonblockingReadPoll::StopListen() {
  is_listening_ = false;
}

} // namespace os_lab1