#include "include/rest.h"
#include "include/http.h"

rest::rest(std::string host, std::string trusted_certificate)
    : host(std::move(host)), service("https"),
      json_content_type("application/json; charset=UTF-8"), httpversion(11),
      trusted_certificate(std::move(trusted_certificate)) {}

rest::format rest::to_json(std::string response) {
  rest::format pt;
  std::stringstream ss(response);
  boost::property_tree::json_parser::read_json(ss, pt);
  return pt;
}

std::string rest::from_json(const rest::format &pt) {
  std::stringstream ss;
  boost::property_tree::json_parser::write_json(ss, pt);
  return ss.str();
}
rest::format rest::get(const char *target) const {
  return to_json(http_get(host.c_str(), service.c_str(), target, httpversion,
                          trusted_certificate));
}

rest::format rest::post(const char *target, const rest::format &pt,
                        rest::status expected) const {
  auto response = http_post(host.c_str(), service.c_str(), target, httpversion,
                            trusted_certificate, std::move(from_json(pt)),
                            json_content_type);

  auto code = std::get<0>(response);
  auto content = std::get<1>(response);
  if (code != expected) {
    auto int_code = static_cast<std::underlying_type_t<decltype(code)>>(code);
    throw std::runtime_error("HTTP " + std::to_string(int_code) + ": " +
                             content);
  }
  return to_json(content);
}