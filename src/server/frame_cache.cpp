#include "frame_cache.h"
#include "base_fun.h"
#include "types.h"

#include <iostream>
#include <sys/sysinfo.h>

#define MAX_CACHE_NUM                       4096
#define FRAME_CACHE_SIZE                    1024

frame_cache* g_frame_cache = nullptr;

frame_cache::frame_cache()
{
    init();
}

frame_cache::~frame_cache()
{
    for (auto it : idle_bufs_)
    {
        delete it.data_;
    }
    for (auto it : used_bufs_)
    {
        delete it.data_;
    }
}

void frame_cache::init()
{
    for (int i = 0; i < MAX_CACHE_NUM; i++)
    {
        buffer_info buf_info{new char[FRAME_CACHE_SIZE]{0}, FRAME_CACHE_SIZE};
        idle_bufs_.push_back(buf_info);
    }
}

bool frame_cache::take_idle_buf(buffer_info& buf_info)
{
    idle_buf_mutex_.lock();
    if (idle_bufs_.empty())
    {
        idle_buf_mutex_.unlock();

        memory_info mem_info;
        get_memory_info(mem_info);

        if(mem_info.free_ >= 512 * 1024 * 1024)
        {
            
            auto buf = new char[FRAME_CACHE_SIZE]{0};
            if(!buf)
                return false;

            buf_info.data_ = buf;
            buf_info.size_ = FRAME_CACHE_SIZE;
        }
        else
        {
            return false;
        }
        
    }
    else
    {
        buf_info = idle_bufs_.front();
        idle_bufs_.pop_front();
        idle_buf_mutex_.unlock();
        return true;
    }
    return true;
}

bool frame_cache::take_used_buf(buffer_info& buf_info)
{
    std::lock_guard<std::mutex> lock_mutex(used_buf_mutex_);
    if (used_bufs_.empty())
        return false;
    buf_info = used_bufs_.front();
    used_bufs_.pop_front();
    return true;
}

void frame_cache::put_used_buf(const buffer_info& buf_info)
{
    std::lock_guard<std::mutex> lock_mutex(used_buf_mutex_);
    used_bufs_.push_back(buf_info);
}

void frame_cache::put_idle_buf(const buffer_info& buf_info)
{
    idle_buf_mutex_.lock();
    if (idle_bufs_.size() > MAX_CACHE_NUM)
    {
        idle_buf_mutex_.unlock();
        delete buf_info.data_;
    }
    else
    {
        idle_bufs_.push_back(buf_info);
        idle_buf_mutex_.unlock();
    }
}

int frame_cache::get_buf_size()
{
    return FRAME_CACHE_SIZE;
}

frame_cache * create_frame_cache()
{
    if(!g_frame_cache)
        g_frame_cache = new frame_cache();
    return g_frame_cache;
}

void destrory_frame_cache()
{
    if(g_frame_cache)
    {
        delete g_frame_cache;
        g_frame_cache = nullptr;
    }
}