#include "fair_share_algorithm.hpp"

namespace os_lab2 {

scheduler_simulator::SchedulingAlgorithm::Result FairShareAlgorithm::operator ()(
    std::size_t                                       simulation_time,
    std::vector<scheduler_simulator::ProcessConfig>&  processes
) {
  return { "None", "None", 0 };
}

} // namespace os_lab2
