#pragma once

#include "attack.h"
#include "enemies.h"
#include "player_list.h"
#include <stddef.h>

int get_socket_line(int socket, char *buffer, size_t buffer_size);
int connect_to_server(char *address, int port);
int get_server_command(int socket, players_t **players,
    particle_arr_t **particles, int *score, attacks_t **attacks,
    enemies_t **enemies);