#pragma once

#include "dynamic_vector.hpp"

namespace dc_lab6 {

template <typename T, std::size_t kRows, std::size_t kColumns>
class DynamicMatrix {
 public:
  using InnerType         = T;

  using Row               = DynamicVector<T, kColumns>;
  using DataContainer     = DynamicVector<Row, kRows>;

  using RowIterator       = typename DataContainer::Iterator;
  using RowConstIterator  = typename DataContainer::ConstIterator;

 public:
  constexpr static std::size_t GetStaticRows() { return kRows; }
  constexpr static std::size_t GetStaticColums() { return kColumns; }

 public:
  DynamicMatrix() noexcept = default;
  explicit DynamicMatrix(const T& scalar) noexcept;
  DynamicMatrix(std::initializer_list<DynamicVector<T, kColumns>> list) noexcept;

  DynamicMatrix(const DynamicMatrix& other) noexcept = default;
  DynamicMatrix(DynamicMatrix&& other) noexcept = default;

 public:
  [[nodiscard]] std::size_t GetRows() const { return kRows; }
  [[nodiscard]] std::size_t GetColumns() const { return kColumns; }

 public:
  Row* GetData() noexcept { return data_.GetData(); }
  const Row* GetData() const noexcept { return data_.GetData(); }

 public:
  [[nodiscard]] RowIterator Begin() noexcept { return data_.Begin(); }
  [[nodiscard]] RowIterator End() noexcept { return data_.End(); }

  [[nodiscard]] RowConstIterator Begin() const noexcept { return data_.Begin(); }
  [[nodiscard]] RowConstIterator End() const noexcept { return data_.End(); }

 public:
  DynamicMatrix& operator =(const DynamicMatrix& other) noexcept = default;
  DynamicMatrix& operator =(DynamicMatrix&& other) noexcept = default;

  Row& operator [](std::size_t index) { return data_[index]; }
  const Row& operator [](std::size_t index) const { return data_[index]; }

  Row* operator &() noexcept { return data_.GetData(); }
  const Row* operator &() const noexcept { return data_.GetData(); }

  bool operator ==(const DynamicMatrix& other) const noexcept;

 public:
  static inline const DynamicMatrix kE = DynamicMatrix(1);

 private:
  DataContainer data_;
};

template <
  typename LhsT, std::size_t kLhsRows, std::size_t kLhsColumns,
  typename RhsT, std::size_t kRhsRows, std::size_t kRhsColumns
>
DynamicMatrix<std::common_type_t<LhsT, RhsT>, kLhsRows, kRhsColumns> operator *(
  const DynamicMatrix<LhsT, kLhsRows, kLhsColumns>& lhs,
  const DynamicMatrix<RhsT, kRhsRows, kRhsColumns>& rhs
);

template <typename T, std::size_t kRows, std::size_t kColumns>
inline auto begin(DynamicMatrix<T, kRows, kColumns>& matrix) {
  return matrix.Begin();
}

template <typename T, std::size_t kRows, std::size_t kColumns>
inline auto end(DynamicMatrix<T, kRows, kColumns>& matrix) {
  return matrix.End();
}

template <typename T, std::size_t kRows, std::size_t kColumns>
inline auto begin(const DynamicMatrix<T, kRows, kColumns>& matrix) {
  return matrix.Begin();
}

template <typename T, std::size_t kRows, std::size_t kColumns>
inline auto end(const DynamicMatrix<T, kRows, kColumns>& matrix) {
  return matrix.End();
}

} // namespace dc_lab6

#include "dynamic_matrix.inl"
