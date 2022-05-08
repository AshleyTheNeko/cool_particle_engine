#pragma once

#include "particle_array.h"

typedef struct fireworks {
    int flight_duration;
    sfVector2f position;
    sfVector2f vector;
    struct fireworks *next;
    struct fireworks *prev;
} fireworks_t;

fireworks_t *firework_create(fireworks_t *fireworks, int flight_duration,
    sfVector2f position, sfVector2f vector);
int firework_refresh(fireworks_t *firework, particle_arr_t **array);
fireworks_t *firework_pop(fireworks_t *array);
fireworks_t *fireworks_refresh(fireworks_t *fireworks, particle_arr_t **array);