#pragma once

#include "particle_array.h"

typedef struct enemey {
    sfVector2f position;
    int speed;
    struct enemey *next;
    struct enemey *prev;
} enemies_t;

enemies_t *enemy_create(enemies_t *enemies, sfVector2f position);
enemies_t *enemy_pop(enemies_t *array);
enemies_t *enemies_refresh(
    enemies_t *enemies, particle_arr_t **array, int screen_height, int *score);
