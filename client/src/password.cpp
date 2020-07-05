#include "include/password.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>

static void stdin_control(bool enable) {
  struct termios tty;
  tcgetattr(STDIN_FILENO, &tty);
  if (!enable)
    tty.c_lflag &= ~ECHO;
  else
    tty.c_lflag |= ECHO;

  (void)tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

SilentTerminal::SilentTerminal() { stdin_control(false); }
SilentTerminal::~SilentTerminal() {
  stdin_control(true);
  std::cout << "\n";
}
