#include <iostream>

#include "matrix.hpp"
#include "matrix_algorithm.hpp"

int main() {
//  constexpr nm_lab2::Matrix<int, 3, 3> coefficients = {
//    decltype(coefficients)::Row(1, 2, 3),
//    decltype(coefficients)::Row(2, 5, 5),
//    decltype(coefficients)::Row(3, 5, 6)
//  };

  constexpr nm_lab2::Matrix<double, 3, 3> coefficients = {
      decltype(coefficients)::Row( 3.0, -1.0, 1.0),
      decltype(coefficients)::Row(-1.0,  2.0, 0.5),
      decltype(coefficients)::Row( 1.0,  0.5, 3.0)
  };

//  constexpr nm_lab2::Vector<int, 3> constant_terms = { 1, 2, 3 };

  constexpr nm_lab2::Vector<double, 3> constant_terms = { 1.0, 1.75, 2.5 };

  auto result = nm_lab2::matrix_algorithm::SeidelMethod(coefficients,constant_terms, 0.001);

  for (auto x : result) {
    std::cout << x << std::endl;
  }
}
