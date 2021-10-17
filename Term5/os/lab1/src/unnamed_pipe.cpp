#include "unnamed_pipe.hpp"

#include <iostream>

#include <unistd.h>

namespace os_lab1 {

UnnamedPipe::UnnamedPipe()
  : file_descriptors_({ 0, 0 }) {}

UnnamedPipe::~UnnamedPipe() {
//  Close();
}

int UnnamedPipe::GetReadDescriptor() const {
  return file_descriptors_[DescriptorType::kRead];
}

int UnnamedPipe::GetWriteDescriptor() const {
  return file_descriptors_[DescriptorType::kWrite];
}

void UnnamedPipe::Open() {
  if (pipe(file_descriptors_.data()) == -1) {
    std::cerr << "Failed to init unnamed pipe" << std::endl;
  }
}

void UnnamedPipe::Close() {
  close(file_descriptors_[DescriptorType::kRead]);
  close(file_descriptors_[DescriptorType::kWrite]);
}

bool UnnamedPipe::operator ==(const UnnamedPipe& other) const {
  return file_descriptors_ == other.file_descriptors_;
}

} // namespace os_lab1