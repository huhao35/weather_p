#pragma once
#include <nlohmann/json.hpp>
#include <jwt-cpp/jwt.h>
#include "jwt-cpp/traits/nlohmann-json/traits.h"
namespace Profile {
using traits = jwt::traits::nlohmann_json;
extern std::string get_historical;
    extern std::string get_current;
    extern std::string api_host;
    extern std::string private_key_path;
    extern std::string kid;
    extern std::string project_id;
    extern std::string issuer_id;
    extern std::string algorithm;
    std::string get_jwt_string();
};
std::string get_url_current(float la, float lo);
std::string get_url_historical(std::chrono::year_month_day ymd,
                               std::string loc = "101010100");    
