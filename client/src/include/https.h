
#pragma once
#include <boost/beast/http/status.hpp>
#include <string>
#include <tuple>

/**
 * Makes a HTTP GET request over secure HTTPS channel.
 * 
 * @param [in] host                     Server host name, using dns or IP address
 * @param [in] port                     Server's listening port, usually 443 but may vary 
 * @param [in] target                   Server's path to the resource (such as "/index.html") 
 * @param [in] version                  HTTP protocol's version (only 11 is tested)
 * @param [in] trusted_certificate      path to a certificate (most likely self signed) trusted by the caller
 * 
 * @return The server's response data, as text
 * 
 * @throw std::runtime_error for any server response other than HTTP 200 (OK) 
 */
std::string https_get(const char *host, const char *port, const char *target,
                      int version, const std::string &trusted_certificate);

/**
 * Makes a HTTP POST request over secure HTTPS channel.
 * 
 * @param [in] host                     Server host name, using dns or IP address
 * @param [in] port                     Server's listening port, usually 443 but may vary 
 * @param [in] target                   Server's path to the resource (such as "/index.html") 
 * @param [in] version                  HTTP protocol's version (only 11 is tested)
 * @param [in] trusted_certificate      path to a certificate (most likely self signed) trusted by the caller
 * @param [in] postdata                 The data for the caller request
 * @param [in] content_type             HTTP type of the caller's request comment (e.g. "application/json; charset=UTF-8")
 * 
 * @return The server's response , both the HTTP code and the attached data, as text 
 */
std::tuple<boost::beast::http::status, std::string>
https_post(const char *host, const char *port, const char *target, int version,
           const std::string &trusted_certificate, std::string postdata,
           std::string content_type);