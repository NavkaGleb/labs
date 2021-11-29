namespace dc_lab6 {

template <typename T, std::size_t kRows, std::size_t kColumns>
DynamicMatrix<T, kRows, kColumns>::DynamicMatrix(const T& scalar) noexcept {
  auto size = std::min(kRows, kColumns);

  for (std::size_t i = 0; i < size; ++i) {
    data_[i][i] = scalar;
  }
}

template <typename T, std::size_t kRows, std::size_t kColumns>
DynamicMatrix<T, kRows, kColumns>::DynamicMatrix(std::initializer_list<DynamicVector<T, kColumns>> list) noexcept {
  for (std::size_t i = 0; i < list.size(); ++i) {
    data_[i] = std::move(*(list.begin() + i));
  }
}

template <typename T, std::size_t kRows, std::size_t kColumns>
bool DynamicMatrix<T, kRows, kColumns>::operator ==(const DynamicMatrix<T, kRows, kColumns>& other) const noexcept {
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
DynamicMatrix<std::common_type_t<LhsT, RhsT>, kLhsRows, kRhsColumns> operator *(
  const DynamicMatrix<LhsT, kLhsRows, kLhsColumns>& lhs,
  const DynamicMatrix<RhsT, kRhsRows, kRhsColumns>& rhs
) {
  static_assert(kLhsColumns == kRhsRows);

  const auto common_dimension = kLhsColumns;

  DynamicMatrix<std::common_type_t<LhsT, RhsT>, kLhsRows, kRhsColumns> result;

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
