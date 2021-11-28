#pragma once

#include <mpi.h>

namespace dc_lab6::mpi_matrix_multiplication {

template <MPI_Datatype kMpiDatatype, typename Matrix>
Matrix Multiply(const Matrix& lhs, const Matrix& rhs, std::size_t process_count);

template <MPI_Datatype kMpiDatatype, typename Matrix>
void TapeMethod(std::size_t process_rank, std::size_t process_count);

template <MPI_Datatype kMpiDatatype, typename Matrix>
void FoxMethod(std::size_t process_rank, std::size_t process_count);

template <MPI_Datatype kMpiDatatype, typename Matrix>
void CannonMethod(std::size_t process_rank, std::size_t process_count);

} // namespace dc_lab6::mpi_matrix_multiplication

#include "mpi_matrix_multiplication.inl"
