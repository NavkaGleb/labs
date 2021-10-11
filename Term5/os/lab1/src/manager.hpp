#ifndef OS_LAB1_MANAGER_HPP_
#define OS_LAB1_MANAGER_HPP_

#include "unnamed_pipe.hpp"
#include "process.hpp"

namespace os_lab1 {

class Manager {
 public:
  Manager();

 private:
  UnnamedPipe pipe_;
  Process     f_process_;
  Process     g_process_;
};

} // namespace os_lab1


#endif // OS_LAB1_MANAGER_HPP_
