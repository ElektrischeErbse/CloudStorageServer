#include <cstring>
#include <gtest/gtest.h>
#include <logger.h>
#include <protocol.h>
#include <string>
#include <vector>

TEST(ProtocolTest, encode_msg_size_match)
{
    Msg msg;
    msg.header.real_size = 10;
    msg.header.total_size = sizeof(MsgHeader) + msg.header.real_size;
    msg.data.assign(10, 'c');
    std::vector<char> data = encode_msg(msg);
    EXPECT_EQ(data.size(), msg.header.total_size);
}

TEST(ProtocolTest, decode_header_data_len_less_than_header_failure)
{
    std::string data = "12345";
    MsgHeader header;
    EXPECT_FALSE(decode_header(data.c_str(), data.length(), header));
}

TEST(ProtocolTest, decode_header_correct_data_success)
{
    Msg msg;
    msg.header.msg_type = MsgType::MSG_LOGIN_REQUEST;
    msg.header.real_size = 10;
    const char *filename = "xxx.log";
    memcpy(msg.header.filename, filename, strlen(filename));
    msg.header.total_size = sizeof(MsgHeader) + msg.header.real_size;
    msg.data.assign(10, 'c');
    std::vector<char> data = encode_msg(msg);

    MsgHeader header;
    EXPECT_TRUE(decode_header(data.data(), data.size(), header));
    EXPECT_EQ(header.real_size, 10);
    EXPECT_EQ(header.total_size, sizeof(MsgHeader) + msg.header.real_size);
    EXPECT_EQ(header.msg_type, MsgType::MSG_LOGIN_REQUEST);
    EXPECT_STREQ(header.filename, filename);
}

TEST(ProtocolTest, decode_msg_data_len_less_than_msg_failure)
{
    std::string data = "12345";
    MsgHeader header;
    EXPECT_FALSE(decode_header(data.c_str(), data.length(), header));
}

TEST(ProtocolTest, decode_msg_correct_data_success)
{
    Msg msg;
    msg.header.msg_type = MsgType::MSG_LOGIN_REQUEST;
    msg.header.real_size = 10;
    const char *filename = "xxx.log";
    memcpy(msg.header.filename, filename, strlen(filename));
    msg.header.total_size = sizeof(MsgHeader) + msg.header.real_size;
    msg.data.assign(10, 'c');
    std::vector<char> data = encode_msg(msg);

    MsgHeader header;
    EXPECT_TRUE(decode_header(data.data(), data.size(), header));
    EXPECT_EQ(header.real_size, 10);
    EXPECT_EQ(header.total_size, sizeof(MsgHeader) + msg.header.real_size);
    EXPECT_EQ(header.msg_type, MsgType::MSG_LOGIN_REQUEST);
    EXPECT_STREQ(header.filename, filename);
    EXPECT_STREQ(msg.data.data(), "cccccccccc");
}

int main(int argc, char *argv[])
{
    Logger::instance().set_level(LogLevel::Off);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
