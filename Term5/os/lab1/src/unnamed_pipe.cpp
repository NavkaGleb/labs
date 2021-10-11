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

UnnamedPipe::~UnnamedPipe() {
  close(file_descriptors_[DescriptorType::kRead]);
  close(file_descriptors_[DescriptorType::kWrite]);
}

} // namespace os_lab1