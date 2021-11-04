#include <iostream>

#include <scheduler_simulator/system.hpp>

#include "first_come_first_served_algorithm.hpp"

int main(int argc, char** argv) {
  scheduler_simulator::CommandLineArgs args(argc, argv);

  auto& scheduling_system = scheduler_simulator::System::GetInstance();

  scheduling_system.Init(args);
  scheduling_system.Run(FirstComeFirstServedAlgorithm());

  std::cin.get();
}
