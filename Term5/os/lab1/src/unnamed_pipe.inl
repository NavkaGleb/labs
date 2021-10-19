#include <stdexcept>
#include <cerrno>
#include <cstring>

#include <unistd.h>

namespace os_lab1 {

template <typename T>
bool UnnamedPipe::ReadSingle(T& data) const {
  auto bytes_count = read(file_descriptors_[DescriptorType::kRead], &data, sizeof(data));

  if (bytes_count == -1) {
    throw std::runtime_error(strerror(errno));
  }

  return bytes_count;
}

template <std::random_access_iterator Iterator>
bool UnnamedPipe::ReadArray(Iterator begin, Iterator end) const {
  auto bytes_count = read(file_descriptors_[DescriptorType::kRead], &*begin, std::distance(begin, end) * sizeof(*begin));

  if (bytes_count == -1) {
    throw std::runtime_error(strerror(errno));
  }

  return bytes_count;
}

template <typename T>
bool UnnamedPipe::WriteSingle(const T& data) const {
  auto bytes_count = write(file_descriptors_[DescriptorType::kWrite], &data, sizeof(data));

  if (bytes_count == -1) {
    throw std::runtime_error(strerror(errno));
  }

  return bytes_count;
}

template <std::random_access_iterator Iterator>
bool UnnamedPipe::WriteArray(Iterator begin, Iterator end) const {
  auto bytes_count = write(file_descriptors_[DescriptorType::kWrite], &*begin, std::distance(begin, end) * sizeof(*begin));

  if (bytes_count == -1) {
    throw std::runtime_error(strerror(errno));
  }

  return bytes_count;
}

} // namespace os_lab1
