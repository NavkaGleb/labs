#include "unnamed_pipe.hpp"

#include <stdexcept>

#include <unistd.h>

namespace os_lab1 {

UnnamedPipe::UnnamedPipe()
  : file_descriptors_({ 0, 0 }) {}

int UnnamedPipe::GetReadDescriptor() const noexcept {
  return file_descriptors_[DescriptorType::kRead];
}

int UnnamedPipe::GetWriteDescriptor() const noexcept {
  return file_descriptors_[DescriptorType::kWrite];
}

void UnnamedPipe::Open() {
  auto status = pipe(file_descriptors_.data());

  if (status == -1) {
    throw std::runtime_error(strerror(errno));
  }
}

void UnnamedPipe::Close() noexcept {
  close(file_descriptors_[DescriptorType::kRead]);
  close(file_descriptors_[DescriptorType::kWrite]);
}

bool UnnamedPipe::operator ==(const UnnamedPipe& other) const noexcept {
  return file_descriptors_ == other.file_descriptors_;
}

} // namespace os_lab1