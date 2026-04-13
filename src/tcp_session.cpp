#include "asio/read.hpp"
#include "asio/write.hpp"
#include "config.h"
#include "err_code.h"
#include "json_handler.h"
#include <asio.hpp>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <logger.h>
#include <protocol.h>
#include <string>
#include <system_error>
#include <tcp_session.h>
#include <vector>


TcpSession::TcpSession(tcp::socket sock) : sock_(std::move(sock)), timer_(sock_.get_executor()) {}

TcpSession::~TcpSession()
{
    // 设置用户状态为离线
    user_service_.set_offline(username_);
    LOG_DEBUG("set_offline({})", username_);
}

void TcpSession::start()
{
    if (!user_service_.init(MYSQL_HOST, MYSQL_USER, MYSQL_PWD, MYSQL_DBNAME, MYSQL_PORT)) {
        return;
    }
    do_read();
}

void TcpSession::do_read()
{
    read_header();
}

void TcpSession::read_header()
{
    auto self = shared_from_this();
    asio::async_read(sock_, asio::buffer(header_buf_.data(), header_buf_.size()), [self, this](std::error_code err, size_t transferred) {
        if (err) {
            LOG_ERROR("read_header error: {}", err.message());
            return;
        }
        handle_header();
    });
}

void TcpSession::handle_header()
{
    if (!decode_header(header_buf_.data(), header_buf_.size(), recv_msg_.header)) {
        LOG_ERROR("decode_header error");
        return;
    }
    uint32_t real_size = recv_msg_.header.real_size;
    data_buf_.resize(real_size);
    read_data();
}

void TcpSession::read_data()
{
    auto self = shared_from_this();
    asio::async_read(sock_, asio::buffer(data_buf_.data(), data_buf_.size()), [self, this](std::error_code err, size_t transferred) {
        if (err) {
            LOG_ERROR("read_data error: {}", err.message());
            return;
        }
        handle_data();
    });
}

void TcpSession::handle_data()
{
    LOG_DEBUG("data: ({})", std::string(data_buf_.begin(), data_buf_.end()));
    recv_msg_.data.assign(data_buf_.begin(), data_buf_.end());
    handle_msg();
}

void TcpSession::handle_msg()
{
    switch (recv_msg_.header.msg_type) {
        case MSG_REGISTER_REQUEST:
            handle_register();
            break;
        case MSG_LOGIN_REQUEST:
            handle_login();
            break;
        default:
            handle_unknown();
            break;
    }
}

void TcpSession::handle_register()
{
    JsonHandler json_handler;
    if (!json_handler.init(std::string(recv_msg_.data.begin(), recv_msg_.data.end()))) {
        prepare_json_response(MSG_REGISTER_RESPONSE, REMOTE_JSON_PARSE_ERROR);
        return;
    }
    std::string username;
    if (!json_handler.get_value("username", username)) {
        prepare_json_response(MSG_REGISTER_RESPONSE, REMOTE_GET_USERNAME_ERROR);
        return;
    }
    std::string password;
    if (!json_handler.get_value("password", password)) {
        prepare_json_response(MSG_REGISTER_RESPONSE, REMOTE_GET_PWD_ERROR);
        return;
    }
    LOG_DEBUG("username: {}, password: {}", username, password);
    if (!user_service_.register_service(username, password)) {
        prepare_json_response(MSG_REGISTER_RESPONSE, REMOTE_REGISTER_USER_ERROR);
        LOG_ERROR("register failed");
        return;
    }
    username_ = username;
    prepare_json_response(MSG_REGISTER_RESPONSE, REGISTER_SUCCESS);
}
void TcpSession::handle_login()
{
    JsonHandler json_handler;
    if (!json_handler.init(std::string(recv_msg_.data.begin(), recv_msg_.data.end()))) {
        prepare_json_response(MSG_LOGIN_RESPONSE, REMOTE_JSON_PARSE_ERROR);
        return;
    }
    std::string username;
    if (!json_handler.get_value("username", username)) {
        prepare_json_response(MSG_LOGIN_RESPONSE, REMOTE_GET_USERNAME_ERROR);
        return;
    }
    std::string password;
    if (!json_handler.get_value("password", password)) {
        prepare_json_response(MSG_LOGIN_RESPONSE, REMOTE_GET_PWD_ERROR);
        return;
    }
    LOG_DEBUG("username: {}, password: {}", username, password);
    if (!user_service_.login_service(username, password)) {
        prepare_json_response(MSG_LOGIN_RESPONSE, REMOTE_LOGIN_USER_ERROR);
        LOG_ERROR("login failed");
        return;
    }
    username_ = username;
    prepare_json_response(MSG_LOGIN_RESPONSE, LOGIN_SUCCESS);
}
void TcpSession::handle_unknown()
{
}

void TcpSession::prepare_json_response(MsgType type, ErrorCode code)
{
    send_msg_.header.msg_type = type;
    memset(send_msg_.header.filename, 0, sizeof(send_msg_.header.filename));
    JsonHandler response;
    if (!response.init("{}")) {
        return;
    }
    if (!response.add_value("code", code)) {
        return;
    }
    if (!response.add_value("err_msg", err_code_msg[code])) {
        return;
    }
    auto str = response.to_string();
    send_msg_.header.real_size = str.size();
    send_msg_.header.total_size = send_msg_.header.real_size + sizeof(MsgHeader);
    send_msg_.data.assign(str.begin(), str.end());
    do_write();
}

void TcpSession::do_write()
{
    auto self = shared_from_this();
    asio::async_write(sock_, asio::buffer(encode_msg(send_msg_)), [self, this](std::error_code err, size_t transferred) {
        if (err) {
            LOG_ERROR("do_write error: {}", err.message());
            return;
        }
        LOG_DEBUG("write msg success");
        do_read();
    });
}

void TcpSession::test_send_msg()
{
    Msg msg;
    msg.header.real_size = 10;
    msg.header.total_size = msg.header.real_size + sizeof(MsgHeader);
    msg.data.assign(10, 'c');
    std::vector<char> data = encode_msg(msg);
    auto self = shared_from_this();
    asio::async_write(sock_, asio::buffer(data), [self, this](std::error_code err, size_t transferred) {
        if (!err) {
            LOG_DEBUG("test send msg success");
        }
    });
}