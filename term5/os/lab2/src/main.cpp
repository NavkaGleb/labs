#include <iostream>

#include "scheduling_system.hpp"
#include "first_come_first_served_algorithm.hpp"

int main(int argc, char** argv) {
  os_lab2::CommandLineArgs args(argc, argv);

  auto& scheduling_system = os_lab2::SchedulingSystem::GetInstance();

  scheduling_system.Init(args);
  scheduling_system.Run(os_lab2::FirstComeFirstServedAlgorithm());

  std::cin.get();
}
