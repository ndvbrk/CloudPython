#pragma once
#include <string>

#include "http.h"
#include <boost/property_tree/json_parser.hpp>
#include <boost/beast/http/status.hpp>

class rest_api {
public:
  rest_api(const char *host, const std::string& trusted_certificate);
  using format = boost::property_tree::ptree; 
  using status = boost::beast::http::status;
  format get(const char *target) const;

  format post(const char *target, const rest_api::format& pt, status expected) const;

  static format to_json(std::string response);
  static std::string from_json(const format& pt);
private:

  const std::string host;
  const std::string service;
  const std::string json_content_type;
  const int httpversion;
  const std::string& trusted_certificate;
};