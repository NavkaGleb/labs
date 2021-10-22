#include "keyboard.hpp"

#include <unistd.h>

namespace os_lab1 {

Keyboard::Keyboard()
  : blocking_settings_()
  , nonblocking_settings_()
  , peek_character_(0) {}

void Keyboard::SwitchToBlockingInput() noexcept {
  tcsetattr(0, TCSANOW, &blocking_settings_);
}

void Keyboard::SwitchToNonblockingInput() noexcept {
  tcgetattr(0, &blocking_settings_);

  nonblocking_settings_             = blocking_settings_;
  nonblocking_settings_.c_lflag    &= ~ICANON;
  nonblocking_settings_.c_lflag    &= ~ECHO;
  nonblocking_settings_.c_lflag    &= ~ISIG;
  nonblocking_settings_.c_cc[VMIN]  = 1;
  nonblocking_settings_.c_cc[VTIME] = 0;

  tcsetattr(0, TCSANOW, &nonblocking_settings_);
  peek_character_ = -1;
}

bool Keyboard::IsPressed() noexcept {
  char ch;

  if (peek_character_ != -1) {
    return true;
  }

  nonblocking_settings_.c_cc[VMIN] = 0;
  tcsetattr(0, TCSANOW, &nonblocking_settings_);

  auto bytes_count = read(0, &ch, 1);

  nonblocking_settings_.c_cc[VMIN] = 1;
  tcsetattr(0, TCSANOW, &nonblocking_settings_);

  if (bytes_count == 1){
    peek_character_ = ch;
    return true;
  }
  
  return false;
}

char Keyboard::GetLastCharacter() noexcept {
  char ch;

  if (peek_character_ == -1) {
    read(0, &ch, 1);
  } else {
    ch              = peek_character_;
    peek_character_ = -1;
  }
  
  return ch;
}

} // namespace oc_lab1
