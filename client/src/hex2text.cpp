

#include "include/hex2text.h"
#include <iomanip>
#include <sstream>
#include <stdexcept>

// File is based on https://stackoverflow.com/a/3790707

// C++98 guarantees that '0', '1', ... '9' are consecutive.
// It only guarantees that 'a' ... 'f' and 'A' ... 'F' are
// in increasing order, but the only two alternative encodings
// of the basic source character set that are still used by
// anyone today (ASCII and EBCDIC) make them consecutive.
static unsigned char hexval(unsigned char c) {
  if ('0' <= c && c <= '9') {
    return c - '0';
  } else if ('a' <= c && c <= 'f') {
    return c - 'a' + 10;
  } else if ('A' <= c && c <= 'F') {
    return c - 'A' + 10;
  }
  throw std::runtime_error("Bad hex format read from server");
}

std::string hex2ascii(const std::string in) {
  std::string result;
  result.reserve(in.length() / 2);
  for (std::string::const_iterator p = in.begin(); p != in.end(); p++) {
    unsigned char c = hexval(*p);
    ++p;
    if (p == in.end()) {
      // incomplete last digit - should report error
      throw std::runtime_error("Bad hex format read from server");
    }
    // + operator takes precedence over << operator
    c = (c << 4) + hexval(*p);
    result.push_back(c);
  }
  return result;
}

std::string bytes2hex(const std::vector<uint8_t> &in) {
  auto from = in.cbegin();
  auto to = in.cend();
  std::ostringstream oss;
  for (; from != to; ++from)
    oss << std::hex << std::setw(2) << std::setfill('0')
        << static_cast<int>(*from);
  return oss.str();
}