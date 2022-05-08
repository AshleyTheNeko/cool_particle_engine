#pragma once

#include "particle.h"

typedef struct particle_node {
    particle_t *particle;
    struct particle_node *prev;
    struct particle_node *next;
} particle_arr_t;

particle_arr_t *append_particle(particle_arr_t *array, particle_t *particle);
particle_arr_t *pop_particle(particle_arr_t *array);
particle_arr_t *refresh_particle_array(particle_arr_t *array);
void display_particle_array(sfRenderWindow *win, particle_arr_t *array);
