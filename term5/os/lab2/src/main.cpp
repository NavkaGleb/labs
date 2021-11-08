#include <iostream>

#include "system.hpp"
#include "sheduler_algorithm/fair_share_algorithm.hpp"

int main(int argc, char** argv) {
  os_lab2::CommandLineArgs args(argc, argv);

  auto& scheduling_system = os_lab2::System::GetInstance();

  scheduling_system.Init(args);
  scheduling_system.Run(os_lab2::FairShareAlgorithm());

  std::cin.get();
}
