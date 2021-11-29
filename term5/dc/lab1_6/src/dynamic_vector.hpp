#pragma once

#include <vector>
#include <initializer_list>
#include <concepts>

namespace dc_lab6 {

template <typename T, std::size_t kSize>
class DynamicVector {
 public:
  using InnerType     = T;

  using DataContainer = std::vector<T>;
  using Iterator      = typename DataContainer::iterator;
  using ConstIterator = typename DataContainer::const_iterator;

 public:
  static std::size_t GetStaticSize() { return kSize; }

 public:
  DynamicVector() noexcept;
  DynamicVector(const T& scalar) noexcept;
  DynamicVector(std::initializer_list<T> list) noexcept;

 public:
  std::size_t GetSize() const noexcept { return data_.size(); }

 public:
  T* GetData() noexcept { return data_.data(); }
  const T* GetData() const noexcept { return data_.data(); }

 public:
  Iterator Begin() noexcept { return data_.begin(); }
  Iterator End() noexcept { return data_.end(); }

  ConstIterator Begin() const noexcept { return data_.begin(); }
  ConstIterator End() const noexcept { return data_.end(); }

 public:
  T& operator [](std::size_t index) { return data_[index]; }
  const T& operator [](std::size_t index) const { return data_[index]; }

  T* operator &() noexcept { return data_.data(); }
  const T* operator &() const noexcept { return data_.data(); }

  bool operator ==(const DynamicVector& other) const noexcept;

 private:
  DataContainer data_;
};

template <typename T, std::size_t kSize>
auto begin(DynamicVector<T, kSize>& vector) {
  return vector.Begin();
}

template <typename T, std::size_t kSize>
auto end(DynamicVector<T, kSize>& vector) {
  return vector.End();
}

template <typename T, std::size_t kSize>
auto begin(const DynamicVector<T, kSize>& vector) {
  return vector.Begin();
}

template <typename T, std::size_t kSize>
auto end(const DynamicVector<T, kSize>& vector) {
  return vector.End();
}

} // namespace dc_lab6

#include "dynamic_vector.inl"
