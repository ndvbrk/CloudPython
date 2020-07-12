
#pragma once
#include <boost/beast/http/status.hpp>
#include <string>
#include <tuple>

/**
 * 
 */
std::string https_get(const char *host, const char *port, const char *target,
                     int version, const std::string &trusted_certificate);

std::tuple<boost::beast::http::status, std::string>
https_post(const char *host, const char *port, const char *target, int version,
          const std::string &trusted_certificate, std::string postdata,
          std::string content_type);