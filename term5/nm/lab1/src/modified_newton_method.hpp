#pragma once

#include <functional>
#include <vector>

namespace nm_lab1 {

class ModifiedNewtonMethod {
 public:
  struct Config {
    double                        min;
    double                        max;
    double                        eps;
    double                        x0;
    std::function<double(double)> f;
    std::function<double(double)> f_derivative1;
  };

 public:
  explicit ModifiedNewtonMethod(Config config);

 public:
  [[nodiscard]] auto GetResult() const -> double;
  [[nodiscard]] auto GetResults() const -> std::vector<double>;

 private:
  Config config_;
};

} // namespace nm_lab1
