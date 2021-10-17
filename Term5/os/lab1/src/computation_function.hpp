#ifndef OS_LAB1_SRC_COMPUTATION_FUNCTION_HPP_
#define OS_LAB1_SRC_COMPUTATION_FUNCTION_HPP_

#include <ostream>
#include <functional>
#include <variant>
#include <optional>

namespace os_lab1 {

enum class ComputationFunctionStatus {
  kUnknown = 0,
  kSoftFail,
  kHardFail,
  kValue
};

inline std::ostream& operator <<(std::ostream& ostream, ComputationFunctionStatus status) {
  switch (status) {
    case ComputationFunctionStatus::kUnknown:   ostream << "Unknown";   break;
    case ComputationFunctionStatus::kSoftFail:  ostream << "SoftFail";  break;
    case ComputationFunctionStatus::kHardFail:  ostream << "HardFail";  break;
    case ComputationFunctionStatus::kValue:     ostream << "Value";     break;
  }

  return ostream;
}

template <typename T>
struct ComputationFunctionResult {
  ComputationFunctionStatus status;
  std::optional<T>          value;

  ComputationFunctionResult()
    : status(ComputationFunctionStatus::kUnknown) {}

  ComputationFunctionResult(ComputationFunctionStatus status)
    : status(status) {}

  ComputationFunctionResult(ComputationFunctionStatus status, const T& value)
      : status(status)
      , value(value) {}
};

template <typename T>
using ComputationFunction = std::function<ComputationFunctionResult<T>(const T&)>;

} // namespace os_lab1

#endif // OS_LAB1_SRC_COMPUTATION_FUNCTION_HPP_
