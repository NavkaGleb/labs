#ifndef OS_LAB1_TESTS_COMPUTATION_FUNCTION_CONVERTER_HPP_
#define OS_LAB1_TESTS_COMPUTATION_FUNCTION_CONVERTER_HPP_

#include <test/probefuncs.hpp>

template <typename Function, typename T>
inline os_lab1::ComputationFunctionResult<T> Convert(const Function& function, const T& x) {
  auto result = function(x);

  if (std::holds_alternative<os::lab1::compfuncs::hard_fail>(result)) {
    return { os_lab1::ComputationFunctionStatus::kHardFail };
  }

  if (std::holds_alternative<os::lab1::compfuncs::soft_fail>(result)) {
    return { os_lab1::ComputationFunctionStatus::kSoftFail };
  }

  return { os_lab1::ComputationFunctionStatus::kValue, std::get<T>(result) };
}

#endif // OS_LAB1_TESTS_COMPUTATION_FUNCTION_CONVERTER_HPP_
