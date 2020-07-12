//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

//------------------------------------------------------------------------------
//
// Example: HTTP SSL client, synchronous
//
//------------------------------------------------------------------------------
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/http/status.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>

#include <cstdlib>
#include <exception>
#include <string>

namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http;   // from <boost/beast/http.hpp>
namespace net = boost::asio;    // from <boost/asio.hpp>
namespace ssl = net::ssl;       // from <boost/asio/ssl.hpp>
using tcp = net::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

http::response<http::string_body>
request(const http::request<http::string_body> &req,
        const tcp::resolver::query &query,
        const std::string &trusted_certificate) {
  // The io_context is required for all I/O
  net::io_context ioc;

  // The SSL context is required, and holds certificates
  ssl::context ctx(ssl::context::tlsv12_client);

  // This holds the root certificate used for verification
  // load_root_certificates(ctx);

  // Verify the remote server's certificate
  ctx.load_verify_file(trusted_certificate);

  // ctx.set_verify_mode(boost::asio::ssl::verify_none);
  ctx.set_verify_mode(ssl::verify_peer);

  // These objects perform our I/O
  tcp::resolver resolver(ioc);
  beast::ssl_stream<beast::tcp_stream> stream(ioc, ctx);

  // Set SNI Hostname (many hosts need this to handshake successfully)
  if (!SSL_set_tlsext_host_name(stream.native_handle(),
                                query.host_name().c_str())) {
    beast::error_code ec{static_cast<int>(::ERR_get_error()),
                         net::error::get_ssl_category()};
    throw beast::system_error{ec};
  }

  // Look up the domain name
  auto const results = resolver.resolve(query);

  // Make the connection on the IP address we get from a lookup
  beast::get_lowest_layer(stream).connect(results);

  // Perform the SSL handshake
  stream.handshake(ssl::stream_base::client);

  // Declare a container to hold the response
  http::response<http::string_body> res;
  // Send the HTTP request to the remote host
  http::write(stream, req);

  // This buffer is used for reading and must be persisted
  beast::flat_buffer buffer;

  // Receive the HTTP response
  http::read(stream, buffer, res);

  // Gracefully close the stream
  beast::error_code ec;
  stream.shutdown(ec);

  // Rationale:
  // http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
  if (ec != net::error::eof && ec != net::ssl::error::stream_truncated) {
    throw beast::system_error{ec};
  }

  return res;
}

std::string https_get(const char *host, const char *port, const char *target,
                      int version, const std::string &trusted_certificate) {
  tcp::resolver::query query(host, port);
  // Set up an HTTP GET request message
  http::request<http::string_body> req{http::verb::get, target, version};
  req.set(http::field::host, host);
  req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

  auto response = request(req, query, trusted_certificate);
  if (http::status::ok == response.base().result()) {
    return response.body();
  }
  std::string error =
      "Bad result: " + std::to_string(response.base().result_int());
  throw std::runtime_error(error);
}

std::tuple<http::status, std::string>
https_post(const char *host, const char *port, const char *target, int version,
           const std::string &trusted_certificate, std::string postdata,
           std::string content_type) {
  tcp::resolver::query query(host, port);
  // Set up an HTTP GET request message
  http::request<http::string_body> req{http::verb::post, target, version};
  req.set(http::field::host, host);
  req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
  req.set(http::field::content_type, content_type);
  req.body() = postdata;
  req.prepare_payload();

  auto response = request(req, query, trusted_certificate);
  auto http_code = response.base().result();
  return std::make_tuple(http_code, response.body());
}