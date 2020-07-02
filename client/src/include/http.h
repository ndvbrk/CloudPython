
#pragma once
#include <boost/beast/http/status.hpp>
#include <string>
#include <tuple>
std::string http_get(const char* host, const char* port, const char* target, int version);

std::tuple<boost::beast::http::status, std::string> http_post(const char* host, const char* port, const char* target, int version, std::string postdata, std::string content_type);