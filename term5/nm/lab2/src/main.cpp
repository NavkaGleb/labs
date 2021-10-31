#include <iostream>

#include "matrix.hpp"
#include "matrix_algorithm.hpp"

int main() {
  constexpr nm_lab2::Matrix<double, 4, 4> coefficients = {
      decltype(coefficients)::Row( 3.0, -1.0, 1.0, 2.0),
      decltype(coefficients)::Row(-1.0,  2.0, 0.5, 1.0),
      decltype(coefficients)::Row( 1.0,  0.5, 3.0, 0.5),
      decltype(coefficients)::Row( 2.0,  1.0, 0.5, 6.0)
  };

  constexpr nm_lab2::Vector<double, 4> constant_terms = { 1.0, 1.75, 2.5, 1.0 };

  auto x = nm_lab2::matrix_algorithm::SeidelMethod(coefficients, constant_terms, 0.001);
//  auto x = nm_lab2::matrix_algorithm::SquareRootMethod(coefficients, constant_terms);

  std::cout << "x: [ ";
  for (auto coefficient : x) {
    std::cout << coefficient << " ";
  }
  std::cout << "]" << std::endl;
}
