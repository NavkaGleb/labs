#pragma once

#include <array>
#include <initializer_list>
#include <concepts>

namespace nm_lab2 {

template <typename T, std::size_t kSize>
class Vector {
 public:
  using DataContainer = std::array<T, kSize>;
  using Iterator      = typename DataContainer::iterator;
  using ConstIterator = typename DataContainer::const_iterator;

 public:
  template <std::same_as<T>... Args>
  constexpr Vector(Args&&... args);

//  constexpr Vector(const Vector& other) = default;
//  constexpr Vector(Vector&& other) noexcept = default;

 public:
  [[nodiscard]] constexpr std::size_t GetSize() const noexcept;

 public:
  [[nodiscard]] constexpr Iterator Begin() noexcept;
  [[nodiscard]] constexpr Iterator End() noexcept;

  [[nodiscard]] constexpr ConstIterator Begin() const noexcept;
  [[nodiscard]] constexpr ConstIterator End() const noexcept;

 public:
  constexpr T& operator [](std::size_t index);
  constexpr const T& operator [](std::size_t index) const;

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

} // namespace nm_lab2

#include "vector.inl"
