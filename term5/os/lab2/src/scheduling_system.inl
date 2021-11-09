#include <iostream>
#include <fstream>

namespace os_lab2 {

template <typename SchedulingAlgorithm>
void SchedulingSystem::Run() {
  std::cout << "Working..." << std::endl;

  auto result = SchedulingAlgorithm()({
    .simulation_time          = simulation_time_,
    .process_configs          = processes_,
    .process_results_filepath = process_results_filepath_
  });

  std::cout << "Algorithm type: " << result.algorithm_type << std::endl;
  std::cout << "Algorithm name: " << result.algorithm_name << std::endl;
  std::cout << "Taken time:     " << result.taken_time     << std::endl;

  std::ofstream out_file(summary_results_filepath_);

  out_file << "Algorithm type: " << result.algorithm_type << std::endl;
  out_file << "Algorithm name: " << result.algorithm_name << std::endl;
  out_file << "Taken time:     " << result.taken_time     << std::endl;

  out_file.close();

  std::cout << "Finish!" << std::endl;
  std::cout << "Check results at '" << summary_results_filepath_ << "' and '" << process_results_filepath_ << "'" << std::endl;
}

} // namespace os_lab2
