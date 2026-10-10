#include <gtest/gtest.h>
#include <chrono>
#include <utils.hpp>
using namespace std::chrono;
TEST(TimeConvert, TimeStampToDateTest) {
year_month_day ymd{floor<days>(sys_seconds{seconds{0}})};
EXPECT_EQ(TimeStampToDate(0), ymd);
}
TEST(TimeConvert, DateToTimeStampTest) {
  EXPECT_EQ(0, DateToTimeStamp("1970-01-01 00:00"));  
}
