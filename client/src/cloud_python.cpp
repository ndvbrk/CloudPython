#include "include/cloud_python.h"
#include "include/hex2text.h"

RegisteredUserData::RegisteredUserData(std::string email,
                                       std::string password) {
  m_data.put("email", email);
  m_data.put("password", password);
}

rest::format RegisteredUserData::data() const { return m_data; }

CloudPython::CloudPython(const rest &node) : node(node){};

RegisteredUser CloudPython::login(std::string username, std::string password) {
  auto user = std::make_unique<RegisteredUserData>(username, password);
  node.post("/api/register", user->data(), rest::status::created);
  return std::move(user);
}

ExecutionResult CloudPython::execute(const RegisteredUser &user,
                                     std::string totp, std::string code) {
  auto data = user->data();
  data.put("totp", totp);
  data.put("data", code);
  auto response = node.post("/api/execute", data, rest::status::ok);

  ExecutionResult result;
  std::string error = response.get<std::string>("error");
  if (error != "ok") {
    throw std::runtime_error("Error:" + error);
  }

  result.exit_code = hex2ascii(response.get<std::string>("exit_code"));
  result.stdout = hex2ascii(response.get<std::string>("stdout"));
  result.stderr = hex2ascii(response.get<std::string>("stderr"));

  return std::move(result);
}
