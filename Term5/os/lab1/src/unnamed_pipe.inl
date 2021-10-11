#include <unistd.h>
#include <iostream>
#include <cassert>

namespace os_lab1 {

template <typename T>
void UnnamedPipe::ReadSingle(T& data) const {
  close(file_descriptors_[DescriptorType::kWrite]);

  auto status = read(file_descriptors_[DescriptorType::kRead], &data, sizeof(data));

  assert(status != -1 && "Failed to read from pipe");
  close(file_descriptors_[DescriptorType::kRead]);
}

template <std::random_access_iterator Iterator>
void UnnamedPipe::ReadArray(Iterator begin, Iterator end) const {
  close(file_descriptors_[DescriptorType::kWrite]);

  auto status = read(file_descriptors_[DescriptorType::kRead], &*begin, std::distance(begin, end) * sizeof(*begin));

  assert(status != -1 && "Failed to read from pipe!");
  close(file_descriptors_[DescriptorType::kRead]);
}

template <typename T>
void UnnamedPipe::WriteSingle(const T& data) const {
  close(file_descriptors_[DescriptorType::kRead]);

  auto status = write(file_descriptors_[DescriptorType::kWrite], &data, sizeof(data));

  assert(status != -1 && "Failed to write to pipe!");
  close(file_descriptors_[DescriptorType::kWrite]);
}

template <std::random_access_iterator Iterator>
void UnnamedPipe::WriteArray(Iterator begin, Iterator end) const {
  close(file_descriptors_[DescriptorType::kRead]);

  auto status = write(file_descriptors_[DescriptorType::kWrite], &*begin, std::distance(begin, end) * sizeof(*begin));

  assert(status != -1 && "Failed to write to pipe!");
  close(file_descriptors_[DescriptorType::kWrite]);
}

} // namespace os_lab1
