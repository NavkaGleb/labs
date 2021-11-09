#pragma once

#include "scheduling_algorithm.hpp"

namespace os_lab2 {

struct FirstComeFirstServedAlgorithm : public SchedulingAlgorithm {
  Result operator ()(Config&& config) override;
};

} // namespace os_lab2

