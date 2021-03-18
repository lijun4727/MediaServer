#include "send_server.h"

#include <boost/asio.hpp>
#include <iostream>

using namespace boost::asio;

send_server::send_server(const char *ip, uint16_t port) : sock_(io_service_),
                                                          server_state_(server_state_idle),
                                                          local_endpoint(ip::address_v4::from_string(ip), port),
                                                          work_(io_service_)
{
    sock_.open(local_endpoint.protocol());
    sock_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
    sock_.bind(local_endpoint);
}

send_server::~send_server()
{
}

bool send_server::run(bool back_run)
{
     if (back_run)
    {
        listen_thread_ = std::thread([this]() {
            this->io_service_.run();
            std::cout << "port:" << local_endpoint.port() << " exit" << std::endl;
        });
    }
    else
    {
        this->io_service_.run();
    }

    return true;
}

void send_server::close()
{
    if(server_state_ == server_state_running)
    {
        sock_.shutdown(socket_base::shutdown_type::shutdown_receive);
        if(!io_service_.stopped())
            io_service_.stop();
        server_state_ = server_state_closed;
    }
}

void send_server::on_write(const boost::system::error_code &err, std::size_t write_bytes)
{
    if(err.failed())
    {
        std::cout << "send failed" << std::endl;
    }
    else
    {
        std::cout << "send success" << std::endl;
    }
    
}

void send_server::send_data(const buffer_info &buf_info, const char *reciever_ip, uint16_t port)
{
    boost::asio::ip::udp::endpoint reciever_ep(ip::address_v4::from_string(reciever_ip), port);
    sock_.async_send_to(buffer(buf_info.data_, buf_info.size_), reciever_ep, std::bind(&send_server::on_write, 
                    shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}