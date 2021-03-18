#pragma once

struct memory_info
{
    int total_;
    int free_;
    int available_;
    int used_;
};

bool get_memory_info(memory_info& mem_info);