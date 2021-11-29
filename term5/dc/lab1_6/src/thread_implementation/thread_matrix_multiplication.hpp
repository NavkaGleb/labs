#pragma once

namespace dc_lab6::thread_matrix_multiplication {

template <typename Matrix>
Matrix TapeMethod(const Matrix& lhs, const Matrix& rhs, std::size_t thread_count);

template <typename Matrix>
Matrix FoxMethod(const Matrix& lhs, const Matrix& rhs, std::size_t thread_count);

template <typename Matrix>
Matrix CannonMethod(const Matrix& lhs, const Matrix& rhs, std::size_t thread_count);

} // namespace dc_lab6::thread_matrix_multiplication

#include "thread_matrix_multiplication.inl"
