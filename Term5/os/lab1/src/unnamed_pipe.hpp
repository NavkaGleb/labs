#ifndef OS_LAB1_SRC_UNNAMED_PIPE_HPP_
#define OS_LAB1_SRC_UNNAMED_PIPE_HPP_

#include <array>
#include <concepts>

namespace os_lab1 {

class UnnamedPipe {
 public:
  UnnamedPipe();
  ~UnnamedPipe();

 public:
  template <typename T>
  void ReadSingle(T& data) const;

  template <std::random_access_iterator Iterator>
  void ReadArray(Iterator begin, Iterator end) const;

  template <typename T>
  void WriteSingle(const T& data) const;

  template <std::random_access_iterator Iterator>
  void WriteArray(Iterator begin, Iterator end) const;

 private:
  enum DescriptorType {
    kRead = 0,
    kWrite
  };

 private:
  std::array<int, 2> file_descriptors_;
};

} // namespace os_lab1

#include "unnamed_pipe.inl"

#endif // OS_LAB1_SRC_UNNAMED_PIPE_HPP_
