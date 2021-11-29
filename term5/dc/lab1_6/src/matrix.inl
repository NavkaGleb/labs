namespace dc_lab6 {

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
constexpr bool Matrix<T, kRows, kColumns>::operator ==(const Matrix<T, kRows, kColumns>& other) const {
  for (std::size_t i = 0; i < kRows; ++i) {
    if (data_[i] != other.data_[i]) {
      return false;
    }
  }

  return true;
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

} // namespace dc_lab6
