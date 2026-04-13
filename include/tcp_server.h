#pragma once

#include <asio.hpp>
#include <cstdint>
#include <memory>

using tcp = asio::ip::tcp;

class TcpServer : public std::enable_shared_from_this<TcpServer> {
public:
    TcpServer(asio::io_context &io, uint16_t port);
    ~TcpServer() = default;
    void do_accept();

private:
    tcp::acceptor acceptor_;
};