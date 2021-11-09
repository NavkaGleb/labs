#include <iostream>

#include "scheduling_system.hpp"
#include "sheduling_algorithms/fair_share_algorithm.hpp"

int main(int argc, char** argv) {
  try {
    os_lab2::CommandLineArgs args(argc, argv);

    auto& system = os_lab2::SchedulingSystem::GetInstance();

    system.Init(args);
    system.Run<os_lab2::FairShareAlgorithm>();
  } catch (const std::exception& exception) {
    std::cerr << exception.what() << std::endl;
  }

  std::cin.get();
}
