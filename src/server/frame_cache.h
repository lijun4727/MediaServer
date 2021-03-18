#pragma once

#include <mutex>
#include <list>

#include "defines.h"

struct buffer_info;

class frame_cache
{
public:
    frame_cache();
    virtual ~frame_cache();

    bool take_idle_buf(buffer_info& buf_info);
    buffer_info take_idle_buf();
    bool take_used_buf(buffer_info& buf_info);
    void put_used_buf(const buffer_info& buf_info);
    void put_idle_buf(const buffer_info& buf_info);
    int get_buf_size();

private:
    void init();

private:
    std::list<buffer_info> idle_bufs_;
    std::list<buffer_info> used_bufs_;
    std::mutex idle_buf_mutex_;
    std::mutex used_buf_mutex_;
};

extern frame_cache* g_frame_cache;

frame_cache * create_frame_cache();

void destrory_frame_cache();