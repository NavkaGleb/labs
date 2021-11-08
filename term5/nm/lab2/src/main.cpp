#include <iostream>

#include "matrix.hpp"
#include "matrix_algorithm.hpp"

int main() {
  auto log_solution = [](const auto& x) {
    std::cout << "x: [";
    std::copy(x.Begin(), x.End(), std::ostream_iterator<double>(std::cout, " "));
    std::cout << "\b]" << std::endl;
  };

  constexpr nm_lab2::Matrix<double, 4, 4> coefficients = {
      decltype(coefficients)::Row( 3.0, -1.0, 1.0, 2.0),
      decltype(coefficients)::Row(-1.0,  2.0, 0.5, 1.0),
      decltype(coefficients)::Row( 1.0,  0.5, 3.0, 0.5),
      decltype(coefficients)::Row( 2.0,  1.0, 0.5, 6.0)
  };

  constexpr nm_lab2::Vector<double, 4> constant_terms = { 1.0, 1.75, 2.5, 1.0 };

  {
    std::cout << "Condition number of matrix" << std::endl;

    auto condition_number = nm_lab2::matrix_algorithm::GetConditionNumber(coefficients);
    std::cout << "condition_number: " << condition_number << std::endl;
    std::cout << std::endl;
  }

  {
    std::cout << "Determinant using square root method" << std::endl;

    auto determinant = nm_lab2::matrix_algorithm::GetDeterminant(coefficients);
    std::cout << "determinant: " << determinant << std::endl;
    std::cout << std::endl;
  }

  {
    std::cout << "Solution using 'square root method'" << std::endl;

    auto x = nm_lab2::matrix_algorithm::SquareRootMethod(coefficients, constant_terms);
    log_solution(x);
    std::cout << std::endl;
  }

  {
    std::cout << "Solution using 'Seidel method' (esp = 0.0001)" << std::endl;

    auto x = nm_lab2::matrix_algorithm::SeidelMethod(coefficients, constant_terms, 0.0001);
    log_solution(x);
    std::cout << std::endl;
  }

  std::cin.get();
}
