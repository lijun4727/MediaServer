#pragma once

#include <string>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/udp.hpp>
#include <thread>

#include "types.h"
#include "frame_cache.h"
#include "handle_message.h"

class recieve_server : public std::enable_shared_from_this<recieve_server>
{
public:
    recieve_server(const char *ip, uint16_t port);
    virtual ~recieve_server();

    bool run(bool back_run = false);
    void close();

private:
    void on_read(const boost::system::error_code &err, std::size_t read_bytes);
    void handle_msg(const buffer_info& buf_info);

private:
    boost::asio::io_service io_service_;
    boost::asio::ip::udp::socket sock_;
    boost::asio::ip::udp::endpoint sender_ep_;
    std::thread listen_thread_;
    ServerState server_state_;
    boost::asio::ip::udp::endpoint local_endpoint;
    frame_cache* frame_cache_;
    buffer_info buf_info_;
    handle_message handle_msg_;
};