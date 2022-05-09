/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** get_socket_line
*/

#include <stdlib.h>
#include <sys/socket.h>

int get_socket_line(int socket, char *buffer, size_t buffer_size)
{
    char last_char = 0;
    int recv_return = 0;
    size_t buffer_offset = 0;

    while ((recv_return = recv(socket, &last_char, 1, MSG_WAITALL)) > 0 &&
        buffer_offset < buffer_size - 1) {
        if (last_char == '\r')
            continue;
        if (last_char == '\n') {
            buffer[buffer_offset] = '\0';
            return (1);
        }
        buffer[buffer_offset] = last_char;
        buffer_offset++;
    }
    return (recv_return);
}
