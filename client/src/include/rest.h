#pragma once
#include <string>

#include "https.h"
#include <boost/beast/http/status.hpp>
#include <boost/property_tree/json_parser.hpp>

class rest {
public:
  using format = boost::property_tree::ptree;
  using status = boost::beast::http::status;

  rest(std::string host, std::string trusted_certificate);
  format get(const char *target) const;

  format post(const char *target, const rest::format &pt,
              status expected) const;

  static format to_json(std::string response);
  static std::string from_json(const format &pt);

private:
  const std::string host;
  const std::string service;
  const std::string json_content_type;
  const int httpversion;
  const std::string trusted_certificate;
};