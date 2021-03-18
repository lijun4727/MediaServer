#include <stdio.h>
#include <stdlib.h>

#include "base_fun.h"

bool get_memory_info(memory_info& mem_info)
{
    FILE *mem_stream;
    char file[64] = {"/proc/meminfo"};
    mem_stream = fopen(file, "r"); 
    if(!mem_stream)
        return false;
    
    char *line_return = nullptr;
    char mem_buffer[256];
    char tmp_itemName[32];

    line_return = fgets(mem_buffer, sizeof(mem_buffer), mem_stream);
    if (!line_return)
        return false;
    sscanf(mem_buffer, "%s%d", tmp_itemName, &mem_info.total_);

    line_return = fgets(mem_buffer, sizeof(mem_buffer), mem_stream);
    if (!line_return)
        return false;
    sscanf(mem_buffer, "%s%d", tmp_itemName, &mem_info.free_);

    line_return = fgets(mem_buffer, sizeof(mem_buffer), mem_stream);
    if (!line_return)
        return false;
    sscanf(mem_buffer, "%s%d", tmp_itemName, &mem_info.available_);

    fclose(mem_stream);

    return true;
}