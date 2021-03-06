#pragma once

#include "rest.h"
#include <memory>

class RegisteredUserData {
public:
  RegisteredUserData(std::string email, std::string password);
  rest::format data() const;

private:
  rest::format m_data;
};

typedef std::unique_ptr<RegisteredUserData> RegisteredUser;

/**
 * This is the result of a successful execution:
 * The client gets the stdout, stderr of his script,
 * along with the exit code.
 * The server does not verify the contents of the exit code file is a number,
 * and send binary data, so the client should or would not make any special
 * effort to not display it outright as it is.
 *
 * This struct is used as it is friendlier to the user
 * than JSON or tuples.
 */
struct ExecutionResult {
  std::string stderr;
  std::string stdout;
  std::string exit_code;
};

/**
 * class for interfacing with the server backend
 */
class CloudPython final {
public:
  /**
   * Constructor.
   * Wraps an existing REST service object,
   * and applies the project's semantics over it (Application layer Protocol).
   */
  CloudPython(rest &&node);

  /**
   * Makes a registration request to the service.
   * if the user is already registered, this has no affect,
   * and the password is not validated by the server,
   * so HTTP code CREATED (201) is returned.
   *
   * If the user is not already registered,
   * the service sends a confirmation email to his inbox,
   * and HTTP code CREATED (201) is returned.
   *
   * returning CREATED(201) is expected in both scenarios,
   * to prevent against an attacker enumerating the system's users,
   * while also violating their privacy.(See
   * https://stackoverflow.com/questions/9269040)
   *
   * @param [in] email      The user's email
   * @param [in] password   The password selected by the user
   *
   * @return On success, the users credentials for future use.
   *
   * @throw std::runtime_error on any error
   */
  RegisteredUser login(std::string email, std::string password);

  /**
   * Makes a code execution request to the service.
   *
   * @param [in] user      The user's credentials
   * @param [in] email      The user's email
   * @param [in] password   The password selected by the user
   *
   * @return On success, the users credentials for future use.
   *
   * @throw std::runtime_error on any error
   */
  ExecutionResult execute(const RegisteredUser &user, std::string totp,
                          std::string code);

private:
  const rest node;
};