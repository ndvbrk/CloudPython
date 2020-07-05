#include "include/input.h"
#include "include/password.h"
#include <iostream>

std::string get_input(std::string prompt) {
  std::string result;
  std::cout << prompt << ":";
  std::getline(std::cin, result);
  std::cout << "\n";
  return result;
}

std::string get_input_hidden(std::string prompt) {
  SilentTerminal terminal_silence;
  return get_input(prompt);
}

bool yes_or_no(std::string prompt) {
  std::string result;
  std::cout << prompt;
  std::getline(std::cin, result);
  if (result == "y" || result == "Y") {
    return true;
  }
  if (result == "n" || result == "N") {
    return false;
  }
  throw std::runtime_error("bad user input");
}