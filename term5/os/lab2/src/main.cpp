#include <iostream>

#include "system.hpp"
#include "sheduling_algorithms/fair_share_algorithm.hpp"
#include "sheduling_algorithms/first_come_first_served_algorithm.hpp"

int main(int argc, char** argv) {
  os_lab2::CommandLineArgs args(argc, argv);

  auto& scheduling_system = os_lab2::System::GetInstance();

  scheduling_system.Init(args);
  scheduling_system.Run(os_lab2::FirstComeFirstServedAlgorithm());

  std::cin.get();
}
