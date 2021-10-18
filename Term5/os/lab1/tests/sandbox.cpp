#include <test/probefuncs.hpp>

#include "manager.hpp"
#include "computation_function_converter.hpp"

int main() {
  os_lab1::ComputationFunction<int> f = [](int x) {
    return Convert(os::lab1::compfuncs::probe_f<os::lab1::compfuncs::INT_SUM>, x);
  };

  os_lab1::ComputationFunction<int> g = [](int x) {
    return Convert(os::lab1::compfuncs::probe_f<os::lab1::compfuncs::INT_SUM>, x);
  };

  os_lab1::Manager manager(f, g);
  manager.Run();
}