#include <iostream>
#include <vector>

#include "platform_detection.hpp"
#include "unnamed_pipe.hpp"

int main() {
  os_lab1::UnnamedPipe pipe;

  if (auto id = fork(); id == 0) {
    std::vector nums = { 1, 2, 3 };

    pipe.WriteArray(nums.begin(), nums.end());
  } else {
    std::vector<int> nums(3);

    pipe.ReadArray(nums.data(), nums.data() + 3);

    std::cout << "Nums: ";

    for (auto num : nums) {
      std::cout << num << " ";
    }

    std::cout << std::endl;
  }

  return 0;
}
