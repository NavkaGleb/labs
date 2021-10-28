#include <iostream>

#include "vector.hpp"
#include "matrix.hpp"

int main() {
  constexpr nm_lab2::Vector<int, 3> vector = { 1, 2, 3 };

  nm_lab2::Matrix<int, 4, 4> matrix = {
    decltype(matrix)::Row{ 1, 2, 3, 4 },
    decltype(matrix)::Row{ 1, 2, 3, 4 },
    decltype(matrix)::Row{ 1, 2, 3, 4 },
    decltype(matrix)::Row{ 1, 2, 3, 4 }
  };

  matrix[0] = { 1, 2, 3, 4 };

  for (const auto& row : matrix) {
    for (const auto& e : row) {
      std::cout << e << " ";
    }

    std::cout << std::endl;
  }
}
