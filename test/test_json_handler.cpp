#include "logger.h"
#include <gtest/gtest.h>
#include <json_handler.h>
#include <string>

TEST(JsonHandlerTest, init_valid_json_success)
{
    JsonHandler json_handler;
    std::string str = R"({
        "username" : "tom",
        "age" : 20
    })";
    EXPECT_TRUE(json_handler.init(str));
}

TEST(JsonHandlerTest, init_invalid_json_failure)
{
    JsonHandler json_handler;
    std::string str = R"({
        "username" : "tom",
        "age" , 20
    })";
    EXPECT_FALSE(json_handler.init(str));
}

TEST(JsonHandlerTest, add_value_not_json_object_failure)
{
    JsonHandler json_handler;
    std::string str = R"([1, 2, 3])";
    ASSERT_TRUE(json_handler.init(str));
    EXPECT_FALSE(json_handler.add_value("tom", "111"));
}

TEST(JsonHandlerTest, add_value_json_object_success)
{
    JsonHandler json_handler;
    std::string str = R"({})";
    ASSERT_TRUE(json_handler.init(str));
    EXPECT_TRUE(json_handler.add_value("tom", "111"));
    EXPECT_TRUE(json_handler.add_value("age", 18));
}

TEST(JsonHandlerTest, get_value_not_json_object_failure)
{
    JsonHandler json_handler;
    std::string str = R"([1, 2, 3])";
    ASSERT_TRUE(json_handler.init(str));
    std::string value;
    EXPECT_FALSE(json_handler.get_value("tom", value));
}

TEST(JsonHandlerTest, get_value_key_not_exist_failure)
{
    JsonHandler json_handler;
    std::string str = R"({
        "username" : "tom",
        "age" : 20
    })";
    ASSERT_TRUE(json_handler.init(str));
    std::string value;
    EXPECT_FALSE(json_handler.get_value("111", value));
    EXPECT_STREQ(value.c_str(), "");
}

TEST(JsonHandlerTest, get_value_key_exist_success)
{
    JsonHandler json_handler;
    std::string str = R"({
        "username" : "tom",
        "age" : 20
    })";
    ASSERT_TRUE(json_handler.init(str));
    std::string value;
    EXPECT_TRUE(json_handler.get_value("username", value));
    EXPECT_STREQ(value.c_str(), "tom");
}

TEST(JsonHandlerTest, to_string_json_format_match)
{
    JsonHandler json_handler;
    std::string str = R"([1,2,3,4,5])";
    ASSERT_TRUE(json_handler.init(str));
    EXPECT_STREQ(json_handler.to_string().c_str(), "[1,2,3,4,5]");
}

int main(int argc, char *argv[])
{
    Logger::instance().set_level(LogLevel::Off);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}