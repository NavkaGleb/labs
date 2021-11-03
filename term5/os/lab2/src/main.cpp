#include <iostream>

#include "scheduling_system.hpp"

os_lab2::SchedulingAlgorithmResult SchedulingAlgorithm(
  std::size_t                     simulation_time,
  std::vector<os_lab2::Process>&  processes
) {
  os_lab2::SchedulingAlgorithmResult result;

  result.algorithm_type = "Batch (NonPreemptive)";
  result.algorithm_name = "First-Come First-Served";

  std::size_t current_process_index = 0;
  std::size_t previous_process_index = 0;

  std::size_t completed = 0;

  while (result.taken_time < simulation_time) {
//    auto process = processes[current_process_index];

    if (processes[current_process_index].cpu_done == processes[current_process_index].cpu_time) {
      completed++;

      if (completed == processes.size()) {
        std::cout << "---------------" << std::endl;
        for (const auto& process : processes) {
          std::cout << process.ToString() << std::endl;
        }

        return result;
      }

      for (std::size_t i = processes.size() - 1; i + 1 > 0; --i) {
        if (processes[i].cpu_done < processes[i].cpu_time) {
          current_process_index = i;
        }
      }
    }

    if (processes[current_process_index].io_blocking == processes[current_process_index].io_next) {
//      std::cout << "Fuck this shit" << std::endl;
//      std::cout << processes[current_process_index].blocked_count << std::endl;
      processes[current_process_index].blocked_count++;
      processes[current_process_index].io_next = 0;

//      std::cout << processes[current_process_index].blocked_count << std::endl;
//      std::cout << "#################" << std::endl;

      previous_process_index = current_process_index;

      for (std::size_t i = processes.size() - 1; i + 1 > 0; --i) {
        if (processes[i].cpu_done < processes[i].cpu_time && previous_process_index != i) {
          current_process_index = i;
        }
      }
    }

    processes[current_process_index].cpu_done++;

    if (processes[current_process_index].io_blocking > 0) {
      processes[current_process_index].io_next++;
    }

    result.taken_time++;
  }

  std::cout << "---------------" << std::endl;
  for (const auto& process : processes) {
    std::cout << process.ToString() << std::endl;
  }

  return result;
}

int main(int argc, char** argv) {
  os_lab2::CommandLineArgs args(argc, argv);

  auto& scheduling_system = os_lab2::SchedulingSystem::GetInstance();

  scheduling_system.Init(args);
  scheduling_system.Run(SchedulingAlgorithm);
}
