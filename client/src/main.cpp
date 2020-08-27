#include "include/cloud_python.h"
#include "include/hex2text.h"
#include "include/input.h"
#include <iostream>

void run(std::string hostname, std::string trusted_cert, std::string script) {
  const std::string green("\033[1;32m");
  const std::string yellow("\033[1;33m");
  const std::string red("\033[1;31m");
  const std::string reset("\033[0m");

  CloudPython service(rest(std::move(hostname), std::move(trusted_cert)));
  auto email = get_input("Enter email");
  auto password = get_input_hidden("Enter password");
  std::cout << "Making register&login request\n";
  auto user = service.login(email, password);

  std::cout << std::endl << yellow;
  std::cout << "If this is the first time you register" << std::endl;
  std::cout << "Check your inbox for a confirmation email with TOTP secret."
            << std::endl;
  std::cout
      << "Do not proceed until your account is authorised by an administator."
      << std::endl;
  std::cout << reset << std::endl;
  auto totp = get_input("Enter TOTP secret");
  auto file_contents = read_file(script);
  auto code = bytes2hex(file_contents);
  std::cout << "Making execution request\n";
  auto result = service.execute(user, totp, code);

  std::cout << "\nresults:\n";
  if (result.exit_code == std::string("0\n")) {
    std::cout << green;
  } else {
    std::cout << red;
  }
  std::cout << "Script exit code:" << result.exit_code << "\n" << reset;
  std::cout << "Script stdout:\n" << green << result.stdout << "\n" << reset;
  std::cout << "Script stderr:\n" << red << result.stderr << "\n" << reset;
  std::cout << "=================================================\n";
  std::cout << std::endl;
}

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cerr << "Usage:\n";
    std::cerr << "\t" << argv[0]
              << " SERVER_HOSTNAME TRUSTED_CERTIFICATE PYTHON_SCRIPT\n";
    return EXIT_FAILURE;
  }
  try {
    run(argv[1], argv[2], argv[3]);
  } catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}