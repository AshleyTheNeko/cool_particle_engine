#include "fireworks.h"
#include <stdlib.h>

fireworks_t *firework_create(fireworks_t *fireworks, int flight_duration,
    sfVector2f position, sfVector2f vector)
{
    fireworks_t *new;

    new = calloc(1, sizeof(fireworks_t));
    if (fireworks)
        fireworks->prev = new;
    new->next = fireworks;
    new->position = position;
    new->flight_duration = flight_duration;
    new->vector = vector;
    return (new);
}

static const sfColor RANDOM_COLORS[] = {{.a = 0xFF, .r = 0xFF, .g = 0, .b = 0},
    {.a = 0xFF, .r = 0, .g = 0xFF, .b = 0},
    {.a = 0xFF, .r = 0, .g = 0, .b = 0xFF},
    {.a = 0xFF, .r = 0, .g = 0xFF, .b = 0xFF},
    {.a = 0xFF, .r = 0xFF, .g = 0, .b = 0xFF},
    {.a = 0xFF, .r = 0xFF, .g = 0xFF, .b = 0}};

int firework_refresh(fireworks_t *firework, particle_arr_t **array)
{
    firework->flight_duration--;
    if (!firework->flight_duration) {
        *array = append_particle(*array,
            create_particles((particle_request_t){
                sfWhite, 100, firework->position, 15, 2, 3, NULL}));
        *array = append_particle(*array,
            create_particles((particle_request_t){RANDOM_COLORS[rand() % 6],
                100, firework->position, 35, 3, 5,
                (refresh_function_t[2]){&refresh_slow, NULL}}));
        *array = append_particle(*array,
            create_particles((particle_request_t){RANDOM_COLORS[rand() % 6],
                100, firework->position, 45, 5, 5,
                (refresh_function_t[2]){&refresh_shrink, NULL}}));
        return (1);
    }
    *array = append_particle(*array,
        create_particles((particle_request_t){
            (sfColor){.a = 0xFF, .r = 25, .g = 25, .b = 25}, 5,
            firework->position, 4, 2, 3, NULL}));
    firework->position.x += firework->vector.x;
    firework->position.y += firework->vector.y;
    return (0);
}

fireworks_t *firework_pop(fireworks_t *array)
{
    fireworks_t *tmp = array ? array->next : NULL;

    if (array->prev)
        array->prev->next = array->next;
    if (array->next)
        array->next->prev = array->prev;
    free(array);
    return (tmp);
}

fireworks_t *fireworks_refresh(fireworks_t *fireworks, particle_arr_t **array)
{
    fireworks_t *tmp = fireworks;

    while (tmp) {
        if (firework_refresh(tmp, array)) {
            if (tmp == fireworks) {
                fireworks = firework_pop(fireworks);
                tmp = fireworks;
            } else
                tmp = firework_pop(tmp);
        } else
            tmp = tmp->next;
    }
    return (fireworks);
}