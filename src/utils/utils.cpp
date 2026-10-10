#include "utils.hpp"
#include <sstream>
std::chrono::year_month_day TimeStampToDate(uint64_t time) {
  std::chrono::sys_seconds tp{std::chrono::seconds(time)};
  std::chrono::zoned_time zt{std::chrono::current_zone(), tp};
  auto dp = std::chrono::floor<std::chrono::days>(tp);
  std::chrono::year_month_day ymd{dp};
  return ymd;
}
std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> result;
    std::istringstream iss(s);
    std::string token;
    while (std::getline(iss, token, delim)) {
        result.push_back(token);
    }
    return result;
}
uint64_t DateToTimeStamp(const std::string &s) {
  std::istringstream iss(s);
  std::chrono::sys_seconds tp;
  iss >> std::chrono::parse("%Y-%m-%d %H:%M", tp);
  auto sec =
      std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch())
          .count();
  return static_cast<uint64_t>(sec);  
}
