#pragma once

#include "player.h"

typedef struct enemies {
    int id;
    int x_pos;
    int y_pos;
    int speed;
    struct enemies *next;
    struct enemies *prev;
} enemies_t;

void enemy_push(enemies_t **enemies, int new_id, int position, int speed);
enemies_t *enemy_pop(enemies_t *array);
enemies_t *enemies_refresh(enemies_t *enemies, players_t *players, int *score);
