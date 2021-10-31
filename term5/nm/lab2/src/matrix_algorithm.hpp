#pragma once

#include <concepts>

#include "matrix_algorithm.hpp"

namespace nm_lab2::matrix_algorithm {

template <typename T>
concept Arithmetic = std::integral<T> || std::floating_point<T>;

template <typename T, std::size_t kRows, std::size_t kColumns>
constexpr bool IsSymmetrical(const Matrix<T, kRows, kColumns>& matrix);

template <typename T, std::size_t kRows, std::size_t kColumns>
constexpr Matrix<T, kColumns, kRows> Transpose(const Matrix<T, kRows, kColumns>& matrix);

template <
  std::size_t kRowBegin,  std::size_t kColumnBegin,
  std::size_t kRowEnd,    std::size_t kColumnEnd,

  typename T,
  std::size_t kRows,      std::size_t kColumns
>
constexpr Matrix<T, kRowEnd - kRowBegin, kColumnEnd - kColumnBegin> SubMatrix(
  const Matrix<T, kRows, kColumns>& matrix
);

template <Arithmetic T, std::size_t kSize>
Vector<T, kSize> SquareRootMethod(
  const Matrix<T, kSize, kSize>&  coefficients,
  const Vector<T, kSize>&         constant_terms
);

template <Arithmetic T, std::size_t kSize>
Vector<T, kSize> SeidelMethod(
  const Matrix<T, kSize, kSize>&  coefficients,
  const Vector<T, kSize>&         constant_terms,
  T                               epsilon
);

template <Arithmetic T, std::size_t kSize>
T GetDeterminant(const Matrix<T, kSize, kSize>& matrix);

} // nm_lab2::matrix_algorithm

#include "matrix_algorithm.inl"
