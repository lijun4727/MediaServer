#include "handle_message.h"
#include <iostream>

using boost::thread;

handle_message::handle_message():
    work_(service_)
{

}

handle_message::~handle_message()
{
    thread_group_.join_all();
}

void handle_message::post(const buffer_info& buf_info)
{
    if (!create_thread_)
    {
        auto cpu_num = boost::thread::hardware_concurrency();
        for (int i = 0; i < cpu_num; ++i)
        {
            thread_group_.create_thread([&]() {
                service_.run();
            });
        }
        create_thread_ = true;
    }
    service_.post(boost::bind(handle, buf_info));
}

void handle_message::stop()
{
    if(!service_.stopped())
        service_.stop();
}

void handle_message::handle(const buffer_info& buf_info)
{
    std::cout << "handle msg:buf_info.data_=" << buf_info.data_ << ",msg.size=" << buf_info.size_ << std::endl;
}