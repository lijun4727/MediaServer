#pragma once

#include <stddef.h>

enum ServerState{
    server_state_idle,
    server_state_running,
    server_state_closed,
};

struct buffer_info
{
    char* data_ = nullptr;
    size_t size_ = 0;
};