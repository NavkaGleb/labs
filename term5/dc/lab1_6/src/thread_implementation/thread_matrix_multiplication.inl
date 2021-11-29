#include <vector>
#include <thread>
#include <cmath>

namespace dc_lab6::thread_matrix_multiplication {

template <typename Matrix>
Matrix TapeMethod(const Matrix& lhs, const Matrix& rhs, std::size_t thread_count
) {
  const auto kSize = Matrix::GetStaticRows();

  Matrix result;

  const auto tape_thread = [&](std::size_t index) {
    auto pivot = static_cast<std::size_t>(std::ceil(static_cast<double>(kSize / static_cast<double>(thread_count))));

    for (std::size_t row = index * pivot; row < (index + 1) * pivot && row < kSize; ++row) {
      std::uint32_t counter = 0;

      while (counter++ < kSize) {
        for (std::size_t i = 0; i < kSize; ++i) {
          result[row][index] += lhs[row][i] * rhs[i][index];
        }

        index = (index + 1) % kSize;
      }
    }
  };

  std::vector<std::thread> threads(thread_count);

  for (std::size_t i = 0; i < threads.size(); ++i) {
    threads[i] = std::thread(tape_thread, i);
  }

  for (auto& thread : threads) {
    thread.join();
  }

  return result;
}

template <typename Matrix>
Matrix FoxMethod(const Matrix& lhs, const Matrix& rhs, std::size_t thread_count) {
  const auto kSize = Matrix::GetStaticRows();

  Matrix result;

  const auto fox_thread = [&](std::size_t index) {
    auto pivot = static_cast<std::size_t>(std::ceil(static_cast<double>(kSize / static_cast<double>(thread_count))));

    for (std::size_t row = index * pivot; row < (index + 1) * pivot && row < kSize; ++row) {
      std::uint32_t counter = 0;

      auto lsh_column = row;
      auto rhs_row    = row;

      while (counter++ < kSize) {
        for (std::size_t i = 0; i < kSize; ++i) {
          result[row][i] += lhs[row][lsh_column] * rhs[rhs_row][i];
        }

        lsh_column  = (lsh_column + 1) % kSize;
        rhs_row     = (rhs_row + 1) % kSize;
      }
    }
  };

  std::vector<std::thread> threads(thread_count);

  for (std::size_t i = 0; i < threads.size(); ++i) {
    threads[i] = std::thread(fox_thread, i);
  }

  for (auto& thread : threads) {
    thread.join();
  }

  return result;
}

template <typename Matrix>
Matrix CannonMethod(const Matrix& lhs, const Matrix& rhs, std::size_t thread_count) {
  const auto kSize = Matrix::GetStaticRows();

  Matrix result;

  const auto cannon_thread = [&](std::size_t index) {
    auto pivot = static_cast<std::size_t>(std::ceil(static_cast<double>(kSize / static_cast<double>(thread_count))));

    for (std::size_t row = index * pivot; row < (index + 1) * pivot && row < kSize; ++row) {
      std::uint32_t counter = 0;

      auto lsh_column = row;
      auto rhs_row    = row;

      while (counter++ < kSize) {
        for (std::size_t i = 0; i < kSize; ++i) {
          result[row][i] += lhs[row][lsh_column] * rhs[rhs_row][i];
        }

        lsh_column  = (kSize + lsh_column - 1) % kSize;
        rhs_row     = (kSize + rhs_row - 1) % kSize;
      }
    }
  };

  std::vector<std::thread> threads(thread_count);

  for (std::size_t i = 0; i < threads.size(); ++i) {
    threads[i] = std::thread(cannon_thread, i);
  }

  for (auto& thread : threads) {
    thread.join();
  }

  return result;
}

} // namespace dc_lab6::mpi_matrix_multiplication
