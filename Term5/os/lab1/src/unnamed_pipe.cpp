#include "unnamed_pipe.hpp"

#include <iostream>

#include <unistd.h>

namespace os_lab1 {

UnnamedPipe::UnnamedPipe()
  : file_descriptors_({ 0, 0 }){

  if (pipe(file_descriptors_.data()) == -1) {
    std::cerr << "Failed to init unnamed pipe" << std::endl;
  }
}

int UnnamedPipe::GetReadDescriptor() const {
  return file_descriptors_[0];
}

int UnnamedPipe::GetWriteDescriptor() const {
  return file_descriptors_[1];
}

} // namespace os_lab1