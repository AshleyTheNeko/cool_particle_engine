#pragma once

#include "particle_array.h"

typedef struct attack {
    sfVector2f position;
    struct attack *next;
    struct attack *prev;
    int id;
} attacks_t;

attacks_t *attack_create(attacks_t *attacks, sfVector2f position, int id);
attacks_t *attack_pop(attacks_t *array);
attacks_t *attacks_refresh(
    attacks_t *attacks, particle_arr_t **array, float time);
