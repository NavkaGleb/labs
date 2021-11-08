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

 public:
  static inline const Matrix kE = Matrix(1);

 private:
  DataContainer data_;
};

template <
  typename LhsT, std::size_t kLhsRows, std::size_t kLhsColumns,
  typename RhsT, std::size_t kRhsRows, std::size_t kRhsColumns
>
constexpr Matrix<std::common_type_t<LhsT, RhsT>, kLhsRows, kRhsColumns> operator *(
  const Matrix<LhsT, kLhsRows, kLhsColumns>& lhs,
  const Matrix<RhsT, kRhsRows, kRhsColumns>& rhs
);

template <
  typename LhsT, std::size_t kLhsRows, std::size_t kLhsColumns,
  typename RhsT, std::size_t kSize
>
constexpr Vector<std::common_type_t<LhsT, RhsT>, kSize> operator *(
    const Matrix<LhsT, kLhsRows, kLhsColumns>&  lhs,
    const Vector<RhsT, kSize>&                  rhs
);

template <typename T, std::size_t kRows, std::size_t kColumns>
constexpr inline auto begin(Matrix<T, kRows, kColumns>& matrix) {
  return matrix.Begin();
}

template <typename T, std::size_t kRows, std::size_t kColumns>
constexpr inline auto end(Matrix<T, kRows, kColumns>& matrix) {
  return matrix.End();
}

template <typename T, std::size_t kRows, std::size_t kColumns>
constexpr inline auto begin(const Matrix<T, kRows, kColumns>& matrix) {
  return matrix.Begin();
}

template <typename T, std::size_t kRows, std::size_t kColumns>
constexpr inline auto end(const Matrix<T, kRows, kColumns>& matrix) {
  return matrix.End();
}

} // namespace nm_lab2

#include "matrix.inl"
