#include "player.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

void player_push(players_t **players, file_descriptor_t player_socket)
{
    players_t *new;

    new = calloc(1, sizeof(players_t));
    if (*players)
        (*players)->prev = new;
    new->next = *players;
    new->socket = player_socket;
    new->position = 25;
    *players = new;
}

players_t *player_pop(players_t *array)
{
    players_t *tmp = array ? array->next : NULL;

    if (array->prev)
        array->prev->next = array->next;
    if (array->next)
        array->next->prev = array->prev;
    free(array);
    return (tmp);
}

__attribute__((format(printf, 2, 3))) void players_broadcast(
    players_t *players, char *fmt, ...)
{
    char buffer[2048] = {0};
    va_list call_args;
    va_start(call_args, fmt);
    vsprintf(buffer, fmt, call_args);
    va_end(call_args);

    while (players) {
        dprintf(players->socket, "%s\n", buffer);
        players = players->next;
    }
}