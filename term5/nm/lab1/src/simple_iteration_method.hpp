#pragma once

#include <functional>
#include <vector>

namespace nm_lab1 {

class SimpleIterationMethod {
 public:
  struct Config {
    double                        min;
    double                        max;
    double                        eps;
    std::function<double(double)> f;
    std::function<double(double)> phi;
    std::function<double(double)> phi_derivative1;
  };

 public:
  explicit SimpleIterationMethod(Config config);

 public:
  [[nodiscard]] size_t GetAprioriEstimate() const;

  [[nodiscard]] auto GetResult() const -> double;
  [[nodiscard]] auto GetResults() const -> std::vector<double>;

 private:
  Config config_;
};

} // namespace nm_lab1
