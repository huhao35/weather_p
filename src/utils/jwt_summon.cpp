#include "jwt_summon.hpp"
#include <chrono>
#include <fstream>
#include <format>
#include <stdexcept>
namespace Profile {
std::string api_host = "https://nb65nqv5pk.re.qweatherapi.com";
std::string get_weather_historical = "/v7/historical/weather";
std::string get_weather_current = "/weather/v1/current/";
std::string private_key_path = "/home/huhao/Documents/WeatherProject/resource/ed25519-private.pem";
std::string kid = "T6H2J6F39X";
std::string project_id = "2NDWA4TH9D";
std::string issuer_id = "Q83874FC95";
std::string algorithm = "EdDSA";
}
std::string Profile::get_jwt_string() {
  std::ifstream ifs;
  ifs.open(private_key_path);
  if (!ifs.is_open()) {
      throw std::runtime_error("Invalid Key Path");
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
std::string get_url_current(float la,float lo) {
  return std::format("{}{}{:.2f}/{:.2f}", Profile::api_host,
                     Profile::get_weather_current, la, lo);
}
std::string get_url_historical(std::chrono::year_month_day ymd,
                               std::string loc) {
    std::string date = std::format("{:04}{:02}{:02}", (int)ymd.year(), (unsigned)ymd.month(),(unsigned)ymd.day());
  return std::format("{}{}?location={}&date={}", Profile::api_host,
                     Profile::get_weather_historical, loc, date);  
}

