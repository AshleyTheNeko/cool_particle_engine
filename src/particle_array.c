#include "particle_array.h"
#include <stdlib.h>

particle_arr_t *append_particle(particle_arr_t *array, particle_t *particle)
{
    particle_arr_t *new;

    new = calloc(1, sizeof(particle_arr_t));
    if (array)
        array->prev = new;
    new->particle = particle;
    new->next = array;
    return (new);
}

// returns next elem or null
particle_arr_t *pop_particle(particle_arr_t *array)
{
    particle_arr_t *tmp = array ? array->next : NULL;

    if (!array)
        return (tmp);
    destroy_particles(array->particle);
    if (array->next)
        array->next->prev = array->prev;
    if (array->prev)
        array->prev->next = array->next;
    free(array);
    return (tmp);
}

particle_arr_t *refresh_particle_array(particle_arr_t *array)
{
    particle_arr_t *tmp = array;

    while (tmp) {
        if (refresh_particles(tmp->particle)) {
            if (tmp == array) {
                array = pop_particle(array);
                tmp = array;
            } else
                tmp = pop_particle(tmp);
        } else
            tmp = tmp->next;
    }
    return (array);
}

void display_particle_array(sfRenderWindow *win, particle_arr_t *array)
{
    particle_arr_t *tmp = array;

    while (tmp) {
        display_particles(tmp->particle, win);
        tmp = tmp->next;
    }
}