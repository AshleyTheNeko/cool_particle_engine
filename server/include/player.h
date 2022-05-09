#pragma once
#include "server_types.h"

typedef struct players {
    file_descriptor_t socket;
    int position;
    struct players *next;
    struct players *prev;
} players_t;

void player_push(players_t **players, file_descriptor_t new_player);
players_t *player_pop(players_t *array);
__attribute__((format(printf, 2, 3))) void players_broadcast(
    players_t *players, char *fmt, ...);
