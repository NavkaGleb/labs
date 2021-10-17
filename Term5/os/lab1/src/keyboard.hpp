#ifndef LAB1_SRC_KEYBOARD_HPP_
#define LAB1_SRC_KEYBOARD_HPP_

#include <termios.h>

namespace os_lab1 {

class Keyboard {
 public:
  Keyboard();

 public:
  void SwitchToBlockingInput();
  void SwitchToNonblockingInput();

  bool IsPressed();
  char GetLastCharacter();

 private:
  termios blocking_settings_;
  termios nonblocking_settings_;
  char    peek_character_;
};

} // namespace oc_lab1

#endif //LAB1_SRC_KEYBOARD_HPP_
