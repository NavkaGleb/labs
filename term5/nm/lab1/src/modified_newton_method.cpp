#include "modified_newton_method.hpp"

namespace nm_lab1 {

ModifiedNewtonMethod::ModifiedNewtonMethod(Config config)
  : config_(std::move(config)) {}

auto ModifiedNewtonMethod::GetResult() const -> double {
  double x0;
  double x1 = config_.x0;

  do {
    x0 = x1;
    x1 = x1 - config_.f(x0) / config_.f_derivative1(config_.max);
  } while (std::abs(x0 - x1) > config_.eps);

  return x1;
}

auto ModifiedNewtonMethod::GetResults() const -> std::vector<double> {
  std::vector<double> results;

  double x0;
  double x1 = config_.x0;

  do {
    x0 = x1;
    x1 = x1 - config_.f(x0) / config_.f_derivative1(config_.max);

    results.push_back(x1);
  } while (std::abs(x0 - x1) > config_.eps);

  return results;
}

} // namespace nm_lab1
