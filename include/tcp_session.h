#pragma once

#include "err_code.h"
#include "protocol.h"
#include "user_service.h"
#include <array>
#include <asio.hpp>
#include <memory>
#include <string>
#include <vector>

using tcp = asio::ip::tcp;

class TcpSession : public std::enable_shared_from_this<TcpSession> {
public:
    TcpSession(tcp::socket sock);
    ~TcpSession();

    void start();
    void test_send_msg();

private:
    /* read msg -> handle msg -> send msg -> read msg */
    void do_read();
    void read_header();
    void handle_header();
    void read_data();
    void handle_data();
    void handle_msg();
    void handle_register();
    void handle_login();
    void handle_unknown();
    void prepare_json_response(MsgType type, ErrorCode code);
    void do_write();

    tcp::socket sock_;
    // 定时器
    asio::steady_timer timer_;
    UserService user_service_;
    std::string username_;
    std::array<char, sizeof(MsgHeader)> header_buf_;
    std::vector<char> data_buf_;

    Msg recv_msg_;
    Msg send_msg_;
};