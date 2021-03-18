#pragma once

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/udp.hpp>
#include <thread>

#include "types.h"

class send_server : public std::enable_shared_from_this<send_server>
{
public:
    send_server(const char *ip, uint16_t port);
    virtual ~send_server();

    bool run(bool back_run = false);
    void close();
    void send_data(const buffer_info& buf_info, const char* reciever_ip, uint16_t port);

private:
    void on_write(const boost::system::error_code &err, std::size_t write_bytes);

private:
    boost::asio::io_service::work work_;
    boost::asio::io_service io_service_;
    boost::asio::ip::udp::socket sock_;
    boost::asio::ip::udp::endpoint local_endpoint;
    ServerState server_state_;
    std::thread listen_thread_;
    buffer_info buf_info_;
};