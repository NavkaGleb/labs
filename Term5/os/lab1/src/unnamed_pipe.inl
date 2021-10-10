#include <unistd.h>
#include <iostream>

namespace os_lab1 {

template <typename T>
void UnnamedPipe::ReadSingle(T& data) const {
  close(GetWriteDescriptor());

  read(GetReadDescriptor(), &data, sizeof(data));
  close(GetReadDescriptor());
}

template <std::random_access_iterator Iterator>
void UnnamedPipe::ReadArray(Iterator begin, Iterator end) const {
  close(GetWriteDescriptor());

  std::cout << std::distance(begin, end) << std::endl;

  std::cout << &*begin << std::endl;

  if (read(GetReadDescriptor(), &*begin, std::distance(begin, end)) == -1) {
    std::cerr << "Failed to read from pipe" << std::endl;
  }

  close(GetReadDescriptor());
}

template <typename T>
void UnnamedPipe::WriteSingle(const T& data) const {
  close(GetReadDescriptor());

  write(GetWriteDescriptor(), &data, sizeof(data));
  close(GetWriteDescriptor());
}

template <std::random_access_iterator Iterator>
void UnnamedPipe::WriteArray(Iterator begin, Iterator end) const {
  close(GetReadDescriptor());

  if (write(GetWriteDescriptor(), &*begin, std::distance(begin, end)) == -1) {
    std::cerr << "Failed to write to pipe" << std::endl;
  }

  close(GetWriteDescriptor());
}

} // namespace os_lab1
