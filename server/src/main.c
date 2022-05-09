#include "server.h"
#include <sys/socket.h>

int main(void)
{
    file_descriptor_t socket_fd = get_server_socket(3333);

    if (socket_fd == -1)
        return (1);
    return (listen_clients(socket_fd));
}