#include "server_types.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <stdio.h>

static struct sockaddr_in get_settings(int port_number)
{
    struct sockaddr_in settings = {0};

    settings.sin_addr = *((struct in_addr[1]){INADDR_ANY});
    settings.sin_family = AF_INET;
    settings.sin_port = htons(port_number);
    return (settings);
}

file_descriptor_t get_server_socket(int port_number)
{
    struct sockaddr_in settings = get_settings(port_number);
    int server = socket(AF_INET, SOCK_STREAM, getprotobyname("TCP")->p_proto);

    if (server == -1) {
        perror("");
        return (-1);
    }
    if (bind(server, (struct sockaddr *)&settings, sizeof(settings)) == -1) {
        perror("");
        return (-1);
    }
    if (listen(server, 50) < 0) {
        perror("");
        return (-1);
    }
    return (server);
}