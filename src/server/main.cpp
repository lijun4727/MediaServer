#include "recieve_server.h"
#include "handle_message.h"

int main()
{
    std::shared_ptr<recieve_server> server = std::make_shared<recieve_server>("0.0.0.0", 51888);
    server->run();
    return 0;
}