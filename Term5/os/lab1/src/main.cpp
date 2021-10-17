#include "manager.hpp"

#include <termios.h>


int main(int /* argc */, char** /* argv */) {
  os_lab1::ComputationFunction<int> f = [](int x) -> os_lab1::ComputationFunctionResult<int> {
    if (x > 24329846) {
      return { os_lab1::ComputationFunctionStatus::kHardFail };
    }

    if (x % 2 == 0) {
      return { os_lab1::ComputationFunctionStatus::kSoftFail };
    }

    return { os_lab1::ComputationFunctionStatus::kValue, x };
  };

  os_lab1::ComputationFunction<int> g = [](int x) -> os_lab1::ComputationFunctionResult<int> {
    if (x > 0) {
      return { os_lab1::ComputationFunctionStatus::kHardFail };
    }

    if (x % 2 == 0) {
      return { os_lab1::ComputationFunctionStatus::kSoftFail };
    }

    return { os_lab1::ComputationFunctionStatus::kValue, x };
  };

  os_lab1::Manager manager(f, g);
  manager.Run();

  return 0;
}