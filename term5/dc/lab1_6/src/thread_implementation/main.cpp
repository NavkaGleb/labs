#include <iostream>
#include <chrono>
#include <fstream>

#include <ng_random/random.hpp>

#include <tabulate/table.hpp>

#include "matrix.hpp"
#include "thread_matrix_multiplication.hpp"

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
auto GetThreadElapsedTime(const Function& function, Args&&... args) {
  auto start = std::chrono::steady_clock::now();

  function(std::forward<Args>(args)...);

  auto end = std::chrono::steady_clock::now();

  return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

template <typename Matrix, typename Function>
double RunMatrixMultiplication(
  std::size_t thread_count,
  std::size_t run_count,
  const Function& function
) {
  std::vector<double> measurements(run_count);

  for (std::size_t i = 0; i < run_count; ++i) {
    auto lhs = GenerateMatrix<Matrix>(-10, 10);
    auto rhs = GenerateMatrix<Matrix>(-10, 10);

    auto elapsed_time = GetThreadElapsedTime(
      function,
      lhs,
      rhs,
      thread_count
    );

    measurements[i] = elapsed_time;

    std::cout << "Matrix" << Matrix::GetStaticRows() << "x" << Matrix::GetStaticColumns()
              << " time: " << elapsed_time << "ms" << std::endl;
  }

  auto average = std::accumulate(
    measurements.begin(),
    measurements.end(),
    0.0
  ) / static_cast<double>(measurements.size());

  return average;
}

template <typename Matrix>
void Run(std::size_t thread_count, std::size_t run_count, tabulate::Table::Row_t& measurements) {
  auto tape   = RunMatrixMultiplication<Matrix>(thread_count, run_count, dc_lab6::thread_matrix_multiplication::TapeMethod<Matrix>);
  auto fox    = RunMatrixMultiplication<Matrix>(thread_count, run_count, dc_lab6::thread_matrix_multiplication::FoxMethod<Matrix>);
  auto cannon = RunMatrixMultiplication<Matrix>(thread_count, run_count, dc_lab6::thread_matrix_multiplication::CannonMethod<Matrix>);

  measurements.push_back(std::to_string(tape));
  measurements.push_back(std::to_string(fox));
  measurements.push_back(std::to_string(cannon));
}

template <typename Matrix>
void PushMeasurements(std::size_t run_count, tabulate::Table& table) {
  tabulate::Table::Row_t measurements;

  measurements.push_back(std::to_string(Matrix::GetStaticRows()) + "x" + std::to_string(Matrix::GetStaticColumns()));

  Run<Matrix>(1, run_count, measurements);
  Run<Matrix>(2, run_count, measurements);
  Run<Matrix>(4, run_count, measurements);

  table.add_row(measurements);
}

int main() {
  using Matrix8x8     = dc_lab6::Matrix<int, 8, 8>;
  using Matrix24x24   = dc_lab6::Matrix<int, 24, 24>;
  using Matrix80x80   = dc_lab6::Matrix<int, 80, 80>;
  using Matrix160x160 = dc_lab6::Matrix<int, 160, 160>;
  using Matrix240x240 = dc_lab6::Matrix<int, 240, 240>;
  using Matrix320x320 = dc_lab6::Matrix<int, 320, 320>;
  using Matrix400x400 = dc_lab6::Matrix<int, 400, 400>;

  const std::size_t kRunCount = 50;

  tabulate::Table table;

  table.add_row({
    "size \\ thread_count",
    "tape 1", "fox 1", "cannon 1",
    "tape 2", "fox 2", "cannon 2",
    "tape 4", "fox 4", "cannon 4"
  });

  PushMeasurements<Matrix8x8>(kRunCount, table);
  PushMeasurements<Matrix24x24>(kRunCount, table);
  PushMeasurements<Matrix80x80>(kRunCount, table);
  PushMeasurements<Matrix160x160>(kRunCount, table);
  PushMeasurements<Matrix240x240>(kRunCount, table);
  PushMeasurements<Matrix320x320>(kRunCount, table);
  PushMeasurements<Matrix400x400>(kRunCount, table);

  std::cout << table << std::endl;

  std::ofstream out_file("assets/threads.txt");
  out_file << table;
  out_file.close();

  std::cin.get();
}
