#include "recieve_server.h"
#include <boost/asio.hpp>
#include <iostream>

using namespace boost::asio;

recieve_server::recieve_server(const char *ip, uint16_t port) : sock_(io_service_),
                                                                server_state_(server_state_idle),
                                                                local_endpoint(ip::address_v4::from_string(ip), port)
{
    sock_.open(local_endpoint.protocol());
    sock_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
    sock_.bind(local_endpoint);

    frame_cache_ = create_frame_cache();
}

recieve_server::~recieve_server()
{
    close();
    if (listen_thread_.joinable())
    {
        listen_thread_.join();
    }

    destrory_frame_cache();
}

bool recieve_server::run(bool back_run)
{
    bool ret = frame_cache_->take_idle_buf(buf_info_);
    if(!ret)
        return false;
    sock_.async_receive_from(buffer(buf_info_.data_, buf_info_.size_), sender_ep_, std::bind(&recieve_server::on_read, shared_from_this(), 
            std::placeholders::_1, std::placeholders::_2));

    server_state_ = server_state_running;

    std::cout << "recieve_server is runing......" << std::endl;

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

void recieve_server::close()
{
    if(server_state_ == server_state_running)
    {
        sock_.shutdown(socket_base::shutdown_type::shutdown_receive);
        if(!io_service_.stopped())
            io_service_.stop();
        server_state_ = server_state_closed;
    }
}

void recieve_server::handle_msg(const buffer_info& buf_info)
{
    handle_msg_.post(buf_info);
}

void recieve_server::on_read(const boost::system::error_code &err, std::size_t read_bytes)
{
    if (server_state_ != server_state_running)
        return;

    auto old_buf_info = buf_info_;
    bool ret = frame_cache_->take_idle_buf(buf_info_);   
    if(ret)
    {
        sock_.async_receive_from(buffer(buf_info_.data_, buf_info_.size_), sender_ep_, std::bind(&recieve_server::on_read, 
                    shared_from_this(), std::placeholders::_1, std::placeholders::_2));       
    }     
    else
    {
        std::cout << "frame_cache_->take_idle_buf failed" << std::endl;
    }
    
    if (read_bytes > 0 && !err.failed())
    {
        handle_msg(old_buf_info);
    }
}