#include <cmath>

namespace dc_lab6::mpi_matrix_multiplication {

template <MPI_Datatype kMpiDatatype, typename Matrix>
Matrix Multiply(const Matrix& lhs, const Matrix& rhs, std::size_t process_count) {
  const auto kSize = Matrix::GetStaticRows();

  MPI_Status  status;
  Matrix      result;

  for (int destination = 1; destination < process_count; ++destination) {
    MPI_Send(&lhs, kSize * kSize, kMpiDatatype, destination, 1, MPI_COMM_WORLD);
    MPI_Send(&rhs, kSize * kSize, kMpiDatatype, destination, 1, MPI_COMM_WORLD);
  }

  auto actual_process_count = process_count - 1;
  auto pivot                = kSize % actual_process_count == 0 ? kSize / actual_process_count : kSize / actual_process_count + 1;

  for (int i = 0; i < pivot; ++i) {
    for (int source = 1; source < process_count; ++source) {
      MPI_Recv(&result[(source - 1) * pivot + i], kSize, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
    }
  }

  return result;
}

template <MPI_Datatype kMpiDatatype, typename Matrix>
void TapeMethod(std::size_t process_rank, std::size_t process_count) {
  const auto kSize = Matrix::GetStaticRows();

  MPI_Status  status;
  Matrix      lhs;
  Matrix      rhs;

  MPI_Recv(&lhs, kSize * kSize, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
  MPI_Recv(&rhs, kSize * kSize, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

  MPI_Request request;

  auto rank   = process_rank - 1;
  auto pivot  = static_cast<std::size_t>(std::ceil(static_cast<double>(kSize / static_cast<double>(process_count - 1))));

  for (std::size_t row = rank * pivot; row < (rank + 1) * pivot && row < kSize; ++row) {
    typename Matrix::Row result_vector;

    for (std::size_t i = 0; i < kSize; ++i) {
      for (std::size_t j = 0; j < kSize; ++j) {
        result_vector[rank] += lhs[row][j] * rhs[j][rank];
      }

      rank = (rank + 1) % kSize;
    }

    MPI_Isend(&result_vector, kSize, kMpiDatatype, 0, 2, MPI_COMM_WORLD, &request);
  }
}

template <MPI_Datatype kMpiDatatype, typename Matrix>
void FoxMethod(std::size_t process_rank, std::size_t process_count) {
  const auto kSize = Matrix::GetStaticRows();

  MPI_Status  status;
  Matrix      lhs;
  Matrix      rhs;

  MPI_Recv(&lhs, kSize * kSize, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
  MPI_Recv(&rhs, kSize * kSize, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

  MPI_Request request;

  auto rank   = process_rank - 1;
  auto pivot  = static_cast<std::size_t>(std::ceil(static_cast<double>(kSize / static_cast<double>(process_count - 1))));

  for (std::size_t row = rank * pivot; row < (rank + 1) * pivot && row < kSize; ++row) {
    typename Matrix::Row result_vector;

    auto lsh_column = row;
    auto rhs_row    = row;

    for (std::size_t i = 0; i < kSize; ++i) {
      for (std::size_t j = 0; j < kSize; ++j) {
        result_vector[j] += lhs[row][lsh_column] * rhs[rhs_row][j];
      }

      lsh_column  = (lsh_column + 1) % kSize;
      rhs_row     = (rhs_row + 1) % kSize;
    }

    MPI_Isend(&result_vector, kSize, kMpiDatatype, 0, 2, MPI_COMM_WORLD, &request);
  }
}

template <MPI_Datatype kMpiDatatype, typename Matrix>
void CannonMethod(std::size_t process_rank, std::size_t process_count) {
  const auto kSize = Matrix::GetStaticRows();

  MPI_Status  status;
  Matrix      lhs;
  Matrix      rhs;

  MPI_Recv(&lhs, kSize * kSize, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
  MPI_Recv(&rhs, kSize * kSize, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

  MPI_Request request;

  auto rank   = process_rank - 1;
  auto pivot  = static_cast<std::size_t>(std::ceil(static_cast<double>(kSize / static_cast<double>(process_count - 1))));

  for (std::size_t row = rank * pivot; row < (rank + 1) * pivot && row < kSize; ++row) {
    typename Matrix::Row result_vector;

    auto lsh_column = row;
    auto rhs_row    = row;

    for (std::size_t i = 0; i < kSize; ++i) {
      for (std::size_t j = 0; j < kSize; ++j) {
        result_vector[j] += lhs[row][lsh_column] * rhs[rhs_row][j];
      }

      lsh_column  = (kSize + lsh_column - 1) % kSize;
      rhs_row     = (kSize + rhs_row - 1) % kSize;
    }

    MPI_Isend(&result_vector, kSize, kMpiDatatype, 0, 2, MPI_COMM_WORLD, &request);
  }
}

} // namespace dc_lab6::mpi_matrix_multiplication