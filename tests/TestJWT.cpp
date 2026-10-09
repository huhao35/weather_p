#include <gtest/gtest.h>
#include <iostream>
#include "jwt_summon.hpp"
class GetTokenTest : public testing::Test {
protected:
  void SetUp() override {
    token = Profile::get_jwt_string();
    project_id = Profile::project_id;
    issuer_id = Profile::issuer_id;
    api_host = Profile::api_host;    
  }  
  void TearDown() override {}
  std::string token;
  std::string project_id;
  std::string issuer_id;
  std::string api_host;  
};
TEST_F(GetTokenTest, GenerateToken) {
  ASSERT_NE(token, "");
  std::cout << token << std::endl;
}
TEST_F(GetTokenTest, TokenCorrect) {
  auto decoded_t = jwt::decode<Profile::traits>(token);  
  EXPECT_EQ(decoded_t.get_issuer(), issuer_id);
  EXPECT_EQ(decoded_t.get_subject(), project_id);
  std:: cout << decoded_t.get_header() << '\n' << decoded_t.get_payload() << std::endl;
}
std::string runCurl(const std::string& url,const std::string& token) {
    std::string cmd = "curl -sS -L --compressed "
                      "-H 'accept: application/json' -H 'Authorization: Bearer "+ token +"' \"" + url + "\" 2>&1";

    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) throw std::runtime_error("popen failed");

    std::string result;
    char buf[4096];
    while (fgets(buf, sizeof(buf), pipe) != nullptr) {
        result += buf;
    }
    int status = pclose(pipe);
    if (status != 0) {
        throw std::runtime_error("curl failed: " + result);
    }
    return result;
}
TEST_F(GetTokenTest, HaveResponse) {
    auto res = runCurl(api_host + "/v7/weather/now?location=116.41,39.92&lang=zh", token);
  EXPECT_NE(res, "");
  std::cout << res << std::endl;
}
