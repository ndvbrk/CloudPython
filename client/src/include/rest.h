#pragma once
#include <string>

#include "https.h"
#include <boost/beast/http/status.hpp>
#include <boost/property_tree/json_parser.hpp>

/**
 * Generic class for talking to a generic REST service over https.
 */
class rest {
public:
  using format = boost::property_tree::ptree;
  using status = boost::beast::http::status;

  /**
   * Constructor.
   * Requires a host name and a path on the filesystem to a trusted certificate
   * to be matched against in the TLS handshake.
   *
   * This allows usage of self signed certificates.
   *
   * The constructor does no actual work except for allocation of members.
   */
  rest(std::string host, std::string trusted_certificate);

  /**
   * Move constructor.
   */
  rest(rest &&other);

  /**
   * Sends a HTTP request of GET type,
   * to the resource specified by the target parameter.
   *
   * @param [in] target: path to the requested server resource
   *
   * @throw std::runtime_error for any response code other than HTTP 200 (OK)
   *
   * @return The server's response payload as a JSON object.
   */
  format get(const char *target) const;

  /**
   * Sends a HTTP request of POST type,
   * to the resource specified by the target parameter.
   *
   * @param [in] target: path to the requested server resource
   * @param [in] pt: payload data for the request, in json/propertytree form
   * @param [in] expected: the server's expected return code
   *
   * @throw std::runtime_error for any response code other that expected
   *
   * @return The server's response payload as a JSON object.
   */
  format post(const char *target, const rest::format &pt,
              status expected) const;

protected:
  /**
   * @return JSON format object by parsing textual input data.
   *
   * @throw an exception for any invalid data.
   */
  static format to_json(std::string response);

  /**
   * @return Textual representation from JSON format.
   *
   * @throw an exception for any invalid data.
   */
  static std::string from_json(const format &pt);

private:
  const std::string host;
  const std::string service;
  const std::string json_content_type;
  const int httpversion;
  const std::string trusted_certificate;
};