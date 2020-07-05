#include "include/cloud_python.h"
#include "include/input.h"
#include <iostream>

void run(std::string hostname, std::string trusted_cert) {
  rest node(std::move(hostname), std::move(trusted_cert));
  CloudPython service(node);
  auto email = get_input("Enter email");
  auto password = get_input_hidden("Enter password");
  std::cout << "Making registration request\n";
  auto user = service.login(email, password);

  while (true) {
    auto totp = get_input("Enter TOTP secret");
    auto code = get_input("Enter code");
    std::cout << "Making execution request\n";
    auto result = service.execute(user, totp, code);

    std::cout << "\nresults:\n";
    std::cout << "Exit code:" << result.exit_code << "\n";
    std::cout << "stdout:" << result.stdout << "\n";
    std::cout << "stderr:" << result.stderr << "\n";
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