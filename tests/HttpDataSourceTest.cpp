#include "gtest/gtest.h"
#include <bits/chrono.h>
#include <chrono>
#include <qeventloop.h>
#include "entity.hpp"
TEST(HttpDataSource, CurrentAPI) {
  QEventLoop loop;
  DataSource::HttpDataSource source;
  std::optional<TempHumidity> res;
  std::string resString;
  bool called = false;
  source.get_data_current([&](std::optional<TempHumidity> a, std::string b) {
    called = true;
    resString = b;
    res = a;
    loop.quit();
  });
  QTimer::singleShot(8000, &loop,&QEventLoop::quit);  
  loop.exec();  
  ASSERT_TRUE(called);
  EXPECT_EQ(resString, "OK") << "resString is:" << resString;
  EXPECT_NE(res, std::nullopt);
}
TEST(HttpDataSource, HistoryAPI) {
    
  QEventLoop loop;
  DataSource::HttpDataSource source;
  std::optional<std::vector<TempHumidity>> res;
  std::string resString;
  bool called = false;
auto now_sec = std::chrono::duration_cast<std::chrono::seconds>(
                                                                std::chrono::system_clock::now().time_since_epoch() - std::chrono::days{3})
                   .count();  
  source.get_data_date(now_sec,[&](std::optional<std::vector<TempHumidity>> a, std::string b) {
    called = true;
    resString = b;
    res = a;
    loop.quit();
  });
  QTimer::singleShot(8000, &loop,&QEventLoop::quit);  
  loop.exec();  
  ASSERT_TRUE(called);
  EXPECT_EQ(resString, "OK") << "resString is:" << resString;
  EXPECT_NE(res, std::nullopt);
  auto resVal = res.value();
  EXPECT_EQ(resVal.size(), 24) << "RES SIZE is: "<< res.value().size();
}
