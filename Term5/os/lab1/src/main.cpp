#include "platform_detection.hpp"

#include "manager.hpp"

#include <thread>
#include <chrono>

#include <Random/Random.hpp>

int main(int /* argc */, char** /* argv */) {
  os_lab1::ComputationFunction<int> f = [](int x) -> os_lab1::ComputationFunctionResult<int> {
    if ((x % 3 == 0 || x % 14 == 1) && Ng::Random::Get<bool>(0.8f)) {
      return { os_lab1::ComputationFunctionStatus::kHardFail };
    }

    if (x < Ng::Random::GetFromRange(-10000, 10000) || x > std::numeric_limits<int>::max() / 10) {
      std::this_thread::sleep_for(std::chrono::milliseconds(Ng::Random::GetFromRange(2000, 3000)));
      return { os_lab1::ComputationFunctionStatus::kSoftFail };
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(Ng::Random::GetFromRange(1000, 3000)));
    return { os_lab1::ComputationFunctionStatus::kValue, x % 1000 };
  };

  os_lab1::ComputationFunction<int> g = [](int x) -> os_lab1::ComputationFunctionResult<int> {
    if ((x % 5 == 2 || x % 7 == 6) && Ng::Random::Get<bool>(0.7f)) {
      return { os_lab1::ComputationFunctionStatus::kHardFail };
    }

    if (x > Ng::Random::GetFromRange(-10000, 10000) || x < Ng::Random::GetFromRange(0, 400) && Ng::Random::Get<bool>(0.5f)) {
      std::this_thread::sleep_for(std::chrono::milliseconds(Ng::Random::GetFromRange(1, 3000)));
      return { os_lab1::ComputationFunctionStatus::kSoftFail };
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(Ng::Random::GetFromRange(500, 2000)));
    return { os_lab1::ComputationFunctionStatus::kValue, x % 1000 };
  };

//  os_lab1::ComputationFunction<int> f = [](int x) -> os_lab1::ComputationFunctionResult<int> {
//    std::this_thread::sleep_for(std::chrono::milliseconds(Ng::Random::GetFromRange(3000, 4000)));
//    return { os_lab1::ComputationFunctionStatus::kValue, x };
//  };
//
//  os_lab1::ComputationFunction<int> g = [](int x) -> os_lab1::ComputationFunctionResult<int> {
//    return { os_lab1::ComputationFunctionStatus::kValue, x };
//  };

  os_lab1::Manager manager(f, g);
  manager.Run();

  return 0;
}