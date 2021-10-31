
#include "matrix.hpp"

namespace nm_lab2 {

template <typename T, std::size_t kRows, std::size_t kColumns>
constexpr Matrix<T, kRows, kColumns>::Matrix(const T& scalar) {
  auto size = std::min(kRows, kColumns);

  for (std::size_t i = 0; i < size; ++i) {
    data_[i][i] = scalar;
  }
}

template <typename T, std::size_t kRows, std::size_t kColumns>
template <std::same_as<Vector<T, kColumns>>... Args>
constexpr Matrix<T, kRows, kColumns>::Matrix(Args&&... args) {
  constexpr auto args_count = sizeof...(args);

  if constexpr (args_count == kRows) {
    data_ = { std::forward<Args>(args)... };
  } else {
    static_assert(false && sizeof...(Args));
  }
}

template <typename T, std::size_t kRows, std::size_t kColumns>
constexpr std::size_t Matrix<T, kRows, kColumns>::GetRows() const {
  return kRows;
}

template <typename T, std::size_t kRows, std::size_t kColumns>
constexpr std::size_t Matrix<T, kRows, kColumns>::GetColumns() const {
  return kColumns;
}

template <typename T, std::size_t kRows, std::size_t kColumns>
constexpr typename Matrix<T, kRows, kColumns>::RowIterator Matrix<T, kRows, kColumns>::Begin() noexcept {
  return data_.Begin();
}

template <typename T, std::size_t kRows, std::size_t kColumns>
constexpr typename Matrix<T, kRows, kColumns>::RowIterator Matrix<T, kRows, kColumns>::End() noexcept {
  return data_.End();
}

template <typename T, std::size_t kRows, std::size_t kColumns>
constexpr typename Matrix<T, kRows, kColumns>::RowConstIterator Matrix<T, kRows, kColumns>::Begin() const noexcept {
  return data_.Begin();
}

template <typename T, std::size_t kRows, std::size_t kColumns>
constexpr typename Matrix<T, kRows, kColumns>::RowConstIterator Matrix<T, kRows, kColumns>::End() const noexcept {
  return data_.End();
}

template <typename T, std::size_t kRows, std::size_t kColumns>
constexpr Vector<T, kColumns>& Matrix<T, kRows, kColumns>::operator[](std::size_t index) {
  return data_[index];
}

template <typename T, std::size_t kRows, std::size_t kColumns>
constexpr const Vector<T, kColumns>& Matrix<T, kRows, kColumns>::operator[](std::size_t index) const {
  return data_[index];
}

template <
    typename LhsT, std::size_t kLhsRows, std::size_t kLhsColumns,
    typename RhsT, std::size_t kRhsRows, std::size_t kRhsColumns
>
constexpr Matrix<std::common_type_t<LhsT, RhsT>, kLhsRows, kRhsColumns> operator *(
    const Matrix<LhsT, kLhsRows, kLhsColumns>& lhs,
    const Matrix<RhsT, kRhsRows, kRhsColumns>& rhs
) {
  static_assert(kLhsColumns == kRhsRows);

  const auto common_dimension = kLhsColumns;

  Matrix<std::common_type_t<LhsT, RhsT>, kLhsRows, kRhsColumns> result;

  for (std::size_t i = 0; i < result.GetRows(); ++i) {
    for (std::size_t j = 0; j < result.GetColumns(); ++j) {
      for (std::size_t k = 0; k < common_dimension; ++k) {
        result[i][j] += lhs[i][k] * rhs[k][j];
      }
    }
  }

  return result;
}

} // namespace nm_lab2
