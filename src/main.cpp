#include "asio/io_context.hpp"
#include "asio/signal_set.hpp"
#include "logger.h"
#include <config.h>
#include <cstdint>
#include <exception>
#include <memory>
#include <tcp_server.h>
#include <unistd.h>
#include <user_dao.h>

int main()
{
    try {
        asio::io_context io;
        asio::signal_set signals(io, SIGINT, SIGTERM);
        signals.async_wait([&io](std::error_code err, int sig) {
            if (!err) {
                LOG_DEBUG("close server");
                io.stop();
            } else {
                LOG_ERROR("async_wait error: {}", err.message());
            }
        });
        std::uint16_t port = SERVER_PORT;
        auto server = std::make_shared<TcpServer>(io, port);
        server->do_accept();
        LOG_DEBUG("server listen on 0.0.0.0:{}", port);
        io.run();
    } catch (const std::exception &e) {
        LOG_ERROR("{}", e.what());
    }
    return 0;
}