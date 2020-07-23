#include "include/input.h"
#include "include/password.h"
#include <fstream>
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

std::vector<uint8_t> read_file(const std::string &filename) {
  // open the file:
  std::ifstream file(filename, std::ios::binary);

  // read the data:
  return std::vector<uint8_t>((std::istreambuf_iterator<char>(file)),
                              std::istreambuf_iterator<char>());
}