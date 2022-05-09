#pragma once

#include "player.h"

typedef struct attacks {
    int id;
    int x_pos;
    int y_pos;
    struct attacks *next;
    struct attacks *prev;
} attacks_t;

void attack_push(attacks_t **attacks, int new_id, int position);
attacks_t *attack_pop(attacks_t *array);
attacks_t *attacks_refresh(attacks_t *attacks, players_t *players);

