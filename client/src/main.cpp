#include "include/hex2text.h"
#include "include/input.h"
#include "include/rest.h"
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>

rest_api::format attempt_register(const rest_api &node) {
  rest_api::format registration_request;

  std::string email = get_input("Enter email");
  std::string password = get_input_hidden("Enter password");
  registration_request.put("email", email);
  registration_request.put("password", password);

  if (yes_or_no("Need to register?")) {
    std::cout << "Making registration request\n";
    node.post("/api/register", registration_request, rest_api::status::created);
  }

  return registration_request;
}
void run(const char *hostname, std::string trusted_cert) {
  rest_api node(hostname, trusted_cert);
  rest_api::format exec_request = attempt_register(node);

  while (true) {
    exec_request.put("totp", get_input("Enter TOTP secret"));
    exec_request.put("data", get_input("Enter code"));
    std::cout << "Making execution request\n";
    auto response =
        node.post("/api/execute", exec_request, rest_api::status::ok);

    std::string error_string = response.get<std::string>("error");
    if (error_string != std::string("ok")) {
      throw std::runtime_error("Error:" + error_string);
    }
    std::cout << "\nresults:\n";
    std::cout << "Exit code:"
              << hex2ascii(response.get<std::string>("exit_code"));
    std::cout << "stdout:" << hex2ascii(response.get<std::string>("stdout"))
              << "\n";
    std::cout << "stderr:\n"
              << hex2ascii(response.get<std::string>("stderr")) << "\n";
    std::cout << "=================================================\n";

    std::cout << std::endl;
  }
}

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "Usage:\n";
    std::cerr << "\t" << argv[0] << " SERVER_HOSTNAME TRUSTED_CERTIFICATE\n";
    return EXIT_FAILURE;
  }
  try {
    run(argv[1], argv[2]);
  } catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}