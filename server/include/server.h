#pragma once

#include "server_types.h"
#include <stddef.h>

file_descriptor_t get_server_socket(int port_number);
int listen_clients(file_descriptor_t server);
int get_socket_line(
    file_descriptor_t socket, char *buffer, size_t buffer_size);