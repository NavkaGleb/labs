#include <iostream>
#include <fstream>

#include <mpi.h>

#include <ng_random/random.hpp>

#include <tabulate/table.hpp>

#include "matrix.hpp"
#include "mpi_matrix_multiplication.hpp"

template <typename Matrix, typename T = typename Matrix::InnerType>
auto GenerateMatrix(T min, T max) {
  Matrix result;

  for (auto& row : result) {
    for (auto& coefficient : row) {
      coefficient = ng::random::Next<T>(min, max);
    }
  }

  return result;
}

template <typename Matrix>
void LogMatrix(const Matrix& matrix) {
  for (const auto& row : matrix) {
    for (const auto& coefficient : row) {
      std::cout << coefficient << "\t";
    }

    std::cout << std::endl;
  }
}

template <typename Function, typename... Args>
double GetMpiElapsedTime(const Function& function, Args&&... args) {
  double start = MPI_Wtime();

  function(std::forward<Args>(args)...);

  return MPI_Wtime() - start;
}

template <typename Matrix, typename Function>
std::optional<double> RunMatrixMultiplication(
  std::size_t process_rank,
  std::size_t process_count,
  std::size_t run_count,
  const Function& function
) {
  std::vector<double> measurements;

  for (std::size_t i = 0; i < run_count; ++i) {
    if (process_rank == 0) {
      auto lhs = GenerateMatrix<Matrix>(-10, 10);
      auto rhs = GenerateMatrix<Matrix>(-10, 10);

      Matrix result;

      auto elapsed_time = GetMpiElapsedTime([&] {
        result = dc_lab6::mpi_matrix_multiplication::Multiply<MPI_INT>(lhs, rhs, process_count);
      });

      std::cout << "Matrix" << Matrix::GetStaticRows() << "x" << Matrix::GetStaticColumns()
                << " time: " << elapsed_time * 1000 << "ms" << std::endl;

      measurements.push_back(elapsed_time * 1000);
    } else {
      function(process_rank, process_count);
    }
  }

  if (process_rank == 0) {
    auto average = std::accumulate(
      measurements.begin(),
      measurements.end(),
      0.0
    ) / static_cast<double>(measurements.size());

    return average;
  }

  return std::nullopt;
}

template <typename Matrix>
void Run(std::size_t process_rank, std::size_t process_count, std::size_t run_count, tabulate::Table::Row_t& measurements) {
  auto tape = RunMatrixMultiplication<Matrix>(
    process_rank,
    process_count,
    run_count,
    dc_lab6::mpi_matrix_multiplication::TapeMethod<MPI_INT, Matrix>
  );

  auto fox = RunMatrixMultiplication<Matrix>(
    process_rank,
    process_count,
    run_count,
    dc_lab6::mpi_matrix_multiplication::FoxMethod<MPI_INT, Matrix>
  );

  auto cannon = RunMatrixMultiplication<Matrix>(
    process_rank,
    process_count,
    run_count,
    dc_lab6::mpi_matrix_multiplication::CannonMethod<MPI_INT, Matrix>
  );

  if (tape) {
    measurements.push_back(std::to_string(*tape));
  }

  if (fox) {
    measurements.push_back(std::to_string(*fox));
  }

  if (cannon) {
    measurements.push_back(std::to_string(*cannon));
  }
}

template <typename Matrix>
void PushMeasurements(std::size_t process_rank, std::size_t process_count, std::size_t run_count, tabulate::Table& table) {
  tabulate::Table::Row_t measurements;

  measurements.push_back(std::to_string(Matrix::GetStaticRows()) + "x" + std::to_string(Matrix::GetStaticColumns()));

  Run<Matrix>(process_rank, process_count, run_count, measurements);

  if (process_rank == 0) {
    table.add_row(measurements);
  }
}

int main(int argc, char** argv) {
  using Matrix8x8     = dc_lab6::Matrix<int, 8, 8>;
  using Matrix24x24   = dc_lab6::Matrix<int, 24, 24>;
  using Matrix80x80   = dc_lab6::Matrix<int, 80, 80>;
  using Matrix160x160 = dc_lab6::Matrix<int, 160, 160>;
  using Matrix240x240 = dc_lab6::Matrix<int, 240, 240>;
  using Matrix320x320 = dc_lab6::Matrix<int, 320, 320>;

  const std::size_t kRunCount = 50;

  MPI_Init(&argc, &argv);

  int process_rank;
  int process_count;

  MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &process_count);

  if (process_count != 2 && process_count != 3 && process_count != 5) {
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }

  auto process_count_string = std::to_string(process_count);

  tabulate::Table table;

  table.add_row({
    "size \\ process_count",
    "tape " + process_count_string,
    "fox " + process_count_string,
    "cannon " + process_count_string
  });

  PushMeasurements<Matrix8x8>(process_rank, process_count, kRunCount, table);
  PushMeasurements<Matrix24x24>(process_rank, process_count, kRunCount, table);
  PushMeasurements<Matrix80x80>(process_rank, process_count, kRunCount, table);
  PushMeasurements<Matrix160x160>(process_rank, process_count, kRunCount, table);
  PushMeasurements<Matrix240x240>(process_rank, process_count, kRunCount, table);
  PushMeasurements<Matrix320x320>(process_rank, process_count, kRunCount, table);

  MPI_Finalize();

  if (process_rank == 0) {
    std::ofstream out_file("assets/mpi_" + process_count_string + ".txt");
    out_file << table;
    out_file.close();

    std::cout << table << std::endl;
    std::cin.get();
  }
}