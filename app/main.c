#include "web_server.h"
#include "logger.h"
#include "parser.h"
#include <unistd.h>

int main(int argc, char* argv[])
{
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    LOG_INFO("Started web server application");
    web_server_t* server;
    web_server_alloc(&server);
    web_server_run(server);
    parser_boot(2);

    sleep(10);
    LOG_DEBUG("Starting closing server");
    web_server_close(server);
    parser_close();
    web_server_free(server);
}
