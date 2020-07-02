#include "include/rest.h"
#include "include/hex2text.h"
#include "include/const.h"
#include "include/password.h"
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>

std::string get_input(std::string prompt) {
  std::string result;
  std::cout << prompt << ":";
  std::getline(std::cin, result);
  std::cout << "\n";
  return result;
}

bool yes_or_no(std::string prompt) {
  std::string result;
  std::cout << prompt;
  std::getline(std::cin, result);
  if (result == "y" || result == "Y") {
    return true;
  }
  if (result == "n" || result == "N") {
    return false;
  }
  throw std::runtime_error("bad user input");
}

rest_api::format attempt_register(const rest_api& node) {
  rest_api::format registeration_request;

  std::string email = get_input("Enter email");
  //std::string email = "ndvbrk@gmail.com";
  enable_silent_input();
  std::string password = get_input("Enter password");
  disable_silent_input();
  //std::string password = "password";
  registeration_request.put("email", email);
  registeration_request.put("password", password);

  if (yes_or_no("Need to register?")) {
    std::cout << "Making registration request\n";
    node.post("/api/register", registeration_request, rest_api::status::created);
  }

  return registeration_request;
}
void run() {
  rest_api node(SERVER_HOSTNAME);
  rest_api::format exec_request = attempt_register(node);
  
  while(true) {
    exec_request.put("totp", get_input("Enter TOTP secret"));
    exec_request.put("data", get_input("Enter code"));
    std::cout << "Making execution request\n";
    auto response = node.post("/api/execute", exec_request, rest_api::status::ok);
    
    std::string error_string = response.get<std::string>("error");
    if (error_string != std::string("ok")) {
      throw std::runtime_error("Error:" + error_string);
    }
    std::cout << "\nresults:\n";
    std::cout << "Exit code:" << hex2ascii(response.get<std::string>("exit_code"));
    std::cout << "stdout:" << hex2ascii(response.get<std::string>("stdout")) << "\n";
    std::cout << "stderr:\n" << hex2ascii(response.get<std::string>("stderr")) << "\n";
    std::cout << "=================================================\n";

    //std::cout << rest_api::from_json(response2) << "\n";
    std::cout << std::endl;
  }
}

int main(int argc, char **argv) {
  try {
    run();
  } catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}