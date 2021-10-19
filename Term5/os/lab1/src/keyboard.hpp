#ifndef OS_LAB1_SRC_KEYBOARD_HPP_
#define OS_LAB1_SRC_KEYBOARD_HPP_

#include <termios.h>

namespace os_lab1 {

class Keyboard {
 public:
  Keyboard();

 public:
  void SwitchToBlockingInput() noexcept;
  void SwitchToNonblockingInput() noexcept;

  bool IsPressed() noexcept;
  char GetLastCharacter() noexcept;

 private:
  termios blocking_settings_;
  termios nonblocking_settings_;
  char    peek_character_;
};

} // namespace oc_lab1

#endif // OS_LAB1_SRC_KEYBOARD_HPP_
