#include <iostream>

#include "platform_detection.hpp"

int main() {
#if defined(OS_LAB1_PLATFORM_WINDOWS)
  std::cout << "Windows!" << std::endl;
#elif defined(OS_LAB1_PLATFORM_LINUX)
  std::cout << "Linux!" << std::endl;
#endif

  return 0;
}
