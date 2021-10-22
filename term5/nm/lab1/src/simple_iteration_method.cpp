#include "simple_iteration_method.hpp"

#include <cmath>

namespace nm_lab1 {

SimpleIterationMethod::SimpleIterationMethod(Config config)
  : config_(std::move(config)) {}

size_t SimpleIterationMethod::GetAprioriEstimate() const {
  const auto kQ = std::max(
    std::abs(config_.phi_derivative1(config_.min)),
    std::abs(config_.phi_derivative1(config_.max))
  );

  return static_cast<size_t>(
    std::log(
      std::abs(config_.phi(config_.max) - config_.max) /
      ((1.0 - kQ) * config_.eps)
    ) / std::log(1.0 / kQ)
  ) + 1;
}

auto SimpleIterationMethod::GetResult() const -> double {
  // |phi_derivative(x)| <= kQ < 1 -> root exists
  // max(|phi_derivative(x)| <= kQ < 1 -> sequence runs down

  const auto kQ = std::max(
    std::abs(config_.phi_derivative1(config_.min)),
    std::abs(config_.phi_derivative1(config_.max))
  );

  std::vector<double> results;

  double x0;
  double x1 = config_.max;

  do {
    x0 = x1;
    x1 = config_.phi(x0);

    results.push_back(x1);
  } while (std::abs(x0 - x1) >= (1 - kQ) / kQ * config_.eps);

  return x1;
}

auto SimpleIterationMethod::GetResults() const -> std::vector<double> {
  // |phi_derivative(x)| <= kQ < 1 -> ok

  const auto kQ = std::max(
    std::abs(config_.phi_derivative1(config_.min)),
    std::abs(config_.phi_derivative1(config_.max))
  );

  std::vector<double> results;

  double x0;
  double x1 = config_.max;

  do {
    x0 = x1;
    x1 = config_.phi(x0);

    results.push_back(x1);
  } while (std::abs(x0 - x1) >= (1 - kQ) / kQ * config_.eps);

  return results;
}

} // namespace nm_lab1
