#include <iostream>

#include <scheduler_simulator/system.hpp>

#include "fair_share_algorithm.hpp"

int main(int argc, char** argv) {
  scheduler_simulator::CommandLineArgs args(argc, argv);

  auto& scheduling_system = scheduler_simulator::System::GetInstance();

  scheduling_system.Init(args);
  scheduling_system.Run(os_lab2::FairShareAlgorithm());

  std::cin.get();
}
