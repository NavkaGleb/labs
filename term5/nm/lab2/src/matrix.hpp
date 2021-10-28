#pragma once

#include "vector.hpp"

namespace nm_lab2 {

template <typename T, std::size_t kRows, std::size_t kColumns>
class Matrix {
 public:
  using Row               = Vector<T, kColumns>;
  using DataContainer     = Vector<Row, kRows>;

  using RowIterator       = typename DataContainer::Iterator;
  using RowConstIterator  = typename DataContainer::ConstIterator;

 public:
  constexpr Matrix() = default;
  constexpr explicit Matrix(const T& scalar);

  template <std::same_as<Row>... Args>
  constexpr Matrix(Args&&... args);

  constexpr Matrix(const Matrix& other) noexcept = default;
  constexpr Matrix(Matrix&& other) noexcept = default;

 public:
  [[nodiscard]] constexpr std::size_t GetRows() const;
  [[nodiscard]] constexpr std::size_t GetColumns() const;

 public:
  [[nodiscard]] constexpr RowIterator Begin() noexcept;
  [[nodiscard]] constexpr RowIterator End() noexcept;

  [[nodiscard]] constexpr RowConstIterator Begin() const noexcept;
  [[nodiscard]] constexpr RowConstIterator End() const noexcept;

 public:
  constexpr Row& operator [](std::size_t index);
  constexpr const Row& operator [](std::size_t index) const;

 private:
  DataContainer data_;
};

template <typename T, std::size_t kRows, std::size_t kColumns>
auto begin(Matrix<T, kRows, kColumns>& matrix) {
  return matrix.Begin();
}

template <typename T, std::size_t kRows, std::size_t kColumns>
auto end(Matrix<T, kRows, kColumns>& matrix) {
  return matrix.End();
}

template <typename T, std::size_t kRows, std::size_t kColumns>
auto begin(const Matrix<T, kRows, kColumns>& matrix) {
  return matrix.Begin();
}

template <typename T, std::size_t kRows, std::size_t kColumns>
auto end(const Matrix<T, kRows, kColumns>& matrix) {
  return matrix.End();
}

} // namespace nm_lab2

#include "matrix.inl"
