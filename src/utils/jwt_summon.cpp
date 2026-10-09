#include "jwt_summon.hpp"
#include <chrono>
#include <fstream>
namespace Profile {
std::string api_host = "https://nb65nqv5pk.re.qweatherapi.com";
std::string private_key_path = "../../resource/ed25519-private.pem";
std::string kid = "T6H2J6F39X";
std::string project_id = "2NDWA4TH9D";
std::string issuer_id = "Q83874FC95";
std::string algorithm = "EdDSA";
}
std::string Profile::get_jwt_string() {
  std::ifstream ifs;
  ifs.open(private_key_path);
  if (!ifs.is_open()) {
      return "";
  }
  std::stringstream ss;
  ss << ifs.rdbuf();
  std::string private_key = ss.str();
  return jwt::create<traits>()
      .set_issuer(issuer_id)
      .set_subject(project_id)
      .set_key_id(kid)
      .set_issued_at(jwt::date::clock::now())
      .set_expires_at(jwt::date::clock::now() + std::chrono::hours(1))
      .sign(jwt::algorithm::ed25519("", private_key));
}

