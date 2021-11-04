#include "common.hpp"

#include <cmath>
#include <iostream>

#include <ng_random/random.hpp>

namespace scheduler_simulator::common {

namespace _detail {

bool R2(double x, double u) {
  return std::pow(x, 2) <= 5.0 - 4.0 * std::exp(0.25) * u;
}

bool R3(double x, double u) {
  return std::pow(x, 2) < 4.0 * std::exp(-1.35) / u + 1.4;
}

bool R4(double x, double u) {
  return std::pow(x, 2) < -4 * std::log(u);
}

} // namespace _detail

double R1() {
  auto u = ng::random::Next<double>(0.0, 1.0);
  auto v = ng::random::Next<double>(0.0, 1.0);

  auto x = std::sqrt(8.0 / std::numbers::e) * (v - 0.5) / u;

  if (!_detail::R2(x, u)) {
    return -1.0;
  }

  if (!_detail::R3(x, u)) {
    return -1.0;
  }

  if (!_detail::R4(x, u)) {
    return -1.0;
  }

  return x;
}

} // namespace scheduler_simulator