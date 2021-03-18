#pragma once

#include "types.h"
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

class handle_message
{
public:
    handle_message();
    virtual ~handle_message();

    void post(const buffer_info &buf_info);
    void stop();

private:
    static void handle(const buffer_info &buf_info);

private:
    boost::asio::io_service service_;
    boost::thread_group thread_group_;
    boost::asio::io_service::work work_;
    bool create_thread_ = false;
};