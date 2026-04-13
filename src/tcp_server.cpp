#include <logger.h>
#include <system_error>
#include <tcp_server.h>
#include <tcp_session.h>

TcpServer::TcpServer(asio::io_context &io, uint16_t port) : acceptor_(io, tcp::endpoint(tcp::v4(), port)) {}

void TcpServer::do_accept()
{
    auto self = shared_from_this();
    acceptor_.async_accept([self, this](std::error_code err, tcp::socket sock) {
        if (!err) {
            LOG_DEBUG("async_accept success [ip: {}] [port: {}]", sock.remote_endpoint().address().to_string(), sock.remote_endpoint().port());
            auto session = std::make_shared<TcpSession>(std::move(sock));
            session->start();
        } else {
            LOG_ERROR("async_accept error: {}", err.message());
        }
        do_accept();
    });
}