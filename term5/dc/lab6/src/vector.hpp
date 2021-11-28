#pragma once

#include <array>
#include <initializer_list>
#include <concepts>

namespace dc_lab6 {

template <typename T, std::size_t kSize>
class Vector {
 public:
  using InnerType     = T;

  using DataContainer = std::array<T, kSize>;
  using Iterator      = typename DataContainer::iterator;
  using ConstIterator = typename DataContainer::const_iterator;

 public:
  constexpr static std::size_t GetStaticSize() { return kSize; }

 public:
  template <std::same_as<T>... Args>
  constexpr Vector(Args&&... args);

 public:
  constexpr std::size_t GetSize() const noexcept { return data_.size(); }

 public:
  constexpr T* GetData() noexcept { return data_.data(); }
  constexpr const T* GetData() const noexcept { return data_.data(); }

 public:
  constexpr Iterator Begin() noexcept { return data_.begin(); }
  constexpr Iterator End() noexcept { return data_.end(); }

  constexpr ConstIterator Begin() const noexcept { return data_.begin(); }
  constexpr ConstIterator End() const noexcept { return data_.end(); }

 public:
  constexpr T& operator [](std::size_t index) { return data_[index]; }
  constexpr const T& operator [](std::size_t index) const { return data_[index]; }

  constexpr bool operator ==(const Vector& other) const;

 private:
  DataContainer data_;
};

template <typename T, std::size_t kSize>
auto begin(Vector<T, kSize>& vector) {
  return vector.Begin();
}

template <typename T, std::size_t kSize>
auto end(Vector<T, kSize>& vector) {
  return vector.End();
}

template <typename T, std::size_t kSize>
auto begin(const Vector<T, kSize>& vector) {
  return vector.Begin();
}

template <typename T, std::size_t kSize>
auto end(const Vector<T, kSize>& vector) {
  return vector.End();
}

} // namespace dc_lab6

#include "vector.inl"
