#include "include/rest.h"
#include "include/http.h"

rest_api::rest_api(const char *host, const std::string& trusted_certificate)
    : host(host), service("https"),
      json_content_type("application/json; charset=UTF-8"), httpversion(11),
      trusted_certificate(trusted_certificate) {}

rest_api::format rest_api::to_json(std::string response){
  rest_api::format pt;
  std::stringstream ss(response);
  boost::property_tree::json_parser::read_json(ss, pt);
  return pt;
}

std::string rest_api::from_json(const rest_api::format& pt){
  std::stringstream ss;
  boost::property_tree::json_parser::write_json(ss, pt);
  return ss.str();
}
rest_api::format rest_api::get(const char *target) const {
  return to_json(http_get(host.c_str(), service.c_str(), target, httpversion, trusted_certificate));
}

rest_api::format rest_api::post(const char *target, const rest_api::format& pt, rest_api::status expected) const {
  auto response = http_post(host.c_str(), service.c_str(), target, httpversion, trusted_certificate,
                   std::move(from_json(pt)), json_content_type);
  
  auto code = std::get<0>(response);
  auto content = std::get<1>(response);
  if (code != expected) {
    auto int_code = static_cast<std::underlying_type_t<decltype(code)>>(code);
    throw std::runtime_error("HTTP " + std::to_string(int_code) +": " + content);
  }
  return to_json(content);
}