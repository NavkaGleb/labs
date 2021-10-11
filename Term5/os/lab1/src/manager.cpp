#include "manager.hpp"

namespace os_lab1 {

Manager::Manager() {
  auto main   = Process::GetCurrent();
  auto child1 = Process::GetCurrent().SpanChild();
  auto child2 = Process::GetCurrent().SpanChild();
  auto child3 = Process::GetCurrent().SpanChild();

//  std::cout << "1. " << child1.GetId() << std::endl;
//  std::cout << "2. " << child2.GetId() << std::endl;
//  std::cout << "3. " << child3.GetId() << std::endl;

  if (child1) {
    auto current = Process::GetCurrent();

    std::cout << "child1.id: " << current.GetId() << ", child1.parent_id: " << current.GetParentId() << std::endl;
  } else if (child2) {
    auto current = Process::GetCurrent();

    std::cout << "child2.id: " << current.GetId() << ", child2.parent_id: " << current.GetParentId() << std::endl;
  } else if (child3) {
    auto current = Process::GetCurrent();

    std::cout << "child3.id: " << current.GetId() << ", child3.parent_id: " << current.GetParentId() << std::endl;
  } else {
    std::cout << "main.id: " << main.GetId() << ", main.parent_id: " << main.GetParentId() << std::endl;
  }
}

} // namespace os_lab1
