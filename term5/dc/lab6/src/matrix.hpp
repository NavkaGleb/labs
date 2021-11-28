#pragma once

#include "vector.hpp"

namespace dc_lab6 {

template <typename T, std::size_t kRows, std::size_t kColumns>
class Matrix {
 public:
  using InnerType         = T;

  using Row               = Vector<T, kColumns>;
  using DataContainer     = Vector<Row, kRows>;

  using RowIterator       = typename DataContainer::Iterator;
  using RowConstIterator  = typename DataContainer::ConstIterator;

 public:
  constexpr static std::size_t GetStaticRows() { return kRows; }
  constexpr static std::size_t GetStaticColumns() { return kColumns; }

 public:
  constexpr Matrix() noexcept = default;
  constexpr explicit Matrix(const T& scalar);

  template <std::same_as<Row>... Args>
  constexpr Matrix(Args&&... args);

  constexpr Matrix(const Matrix& other) noexcept = default;
  constexpr Matrix(Matrix&& other) noexcept = default;

 public:
  constexpr std::size_t GetRows() const { return kRows; }
  constexpr std::size_t GetColumns() const { return kColumns; }

 public:
 public:
  constexpr T* GetData() noexcept { return data_.GetData(); }
  constexpr const T* GetData() const noexcept { return data_.GetData(); }

 public:
  constexpr RowIterator Begin() noexcept { return data_.Begin(); }
  constexpr RowIterator End() noexcept { return data_.End(); }

  constexpr RowConstIterator Begin() const noexcept { return data_.Begin(); }
  constexpr RowConstIterator End() const noexcept { return data_.End(); }

 public:
  constexpr Matrix& operator =(const Matrix& other) noexcept = default;
  constexpr Matrix& operator =(Matrix&& other) noexcept = default;

  constexpr Row& operator [](std::size_t index) { return data_[index]; }
  constexpr const Row& operator [](std::size_t index) const { return data_[index]; }

  constexpr bool operator ==(const Matrix& other) const;

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

} // namespace dc_lab6

#include "matrix.inl"
