
#pragma once
#include <string>
#include <vector>

/**
 * @param [in] in:  hex dump of bytes,
 *                  represented by ASCII characters 0-9,A-F,
 *                  of even length,
 * @return          string representation with ASCII characters,
 *                  ending with a null terminator.
 *
 * @throw std::runtime_error upon any invalid input detected.
 */
std::string hex2ascii(const std::string in);

/**
 * @param [in] in:  byte sequence
 * 
 * @return          hex dump of bytes,
 *                  represented by ASCII characters 0-9,A-F,
 *                  of even length,
 *
 * @throw std::runtime_error upon any invalid input detected.
 */
std::string bytes2hex(const std::vector<uint8_t> &in);