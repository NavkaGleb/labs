#include <iostream>
#include <cmath>
#include <iomanip>

#include "simple_iteration_method.hpp"
#include "modified_newton_method.hpp"

void RunSimpleIterationMethod() {
  std::cout << "SimpleIterationMethod" << std::endl;

  nm_lab1::SimpleIterationMethod::Config config;
  config.min              = 1.0;
  config.max              = 2.0;
  config.eps              = 1e-4;
  config.f                = [](double x) { return std::pow(x, 3) - 3 * std::pow(x, 2) - 17 * x + 22; };
  config.phi              = [](double x) { return (std::pow(x, 3) - 3 * std::pow(x, 2) + 22) / 17.0; };
  config.phi_derivative1  = [](double x) { return (3 * std::pow(x, 2) - 6 * x) / 17.0; };

  nm_lab1::SimpleIterationMethod simple_iteration_method(config);

  std::cout << "Result: " << simple_iteration_method.GetResult() << std::endl;

  auto results = simple_iteration_method.GetResults();

  for (size_t i = 0; i < results.size(); ++i) {
    std::cout << i << std::fixed << std::setprecision(7);
    std::cout << ":\tx = " << results[i];
    std::cout << ", f(x) = " << config.f(results[i]) << std::endl;
  }

  std::cout << "AprioriEstimate: " << simple_iteration_method.GetAprioriEstimate() << std::endl;
  std::cout << std::endl;
}

void RunModifiedNewtonMethod() {
  std::cout << "ModifiedNewtonMethod" << std::endl;

  nm_lab1::ModifiedNewtonMethod::Config config;
  config.min            = 5.0;
  config.max            = 6.0;
  config.eps            = 1e-4;
  config.x0             = 6.0;
  config.f              = [](double x) { return std::pow(x, 3) - 3 * std::pow(x, 2) - 17 * x + 22; };
  config.f_derivative1  = [](double x) { return 3 * std::pow(x, 2) - 6 * x - 17; };

  nm_lab1::ModifiedNewtonMethod modified_newton_method(config);

  std::cout << "Result: " << modified_newton_method.GetResult() << std::endl;

  auto results = modified_newton_method.GetResults();

  for (size_t i = 0; i < results.size(); ++i) {
    std::cout << i << std::fixed << std::setprecision(7);
    std::cout << ":\tx = " << results[i];
    std::cout << ", f(x) = " << config.f(results[i]) << std::endl;
  }

  std::cout << "AprioriEstimate: No estimate!" << std::endl;
  std::cout << std::endl;
}

int main() {
  RunSimpleIterationMethod();
  RunModifiedNewtonMethod();
}
