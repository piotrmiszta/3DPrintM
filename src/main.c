#include "web_server.h"
#include "logger.h"

int main(int argc, char* argv[])
{
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    LOG_INFO("Started web server application");
    web_server_t* server;
    web_server_alloc(&server);
    web_server_run(server);
    while(1) {};
}
