#include "enemies.h"
#include <stdlib.h>

enemies_t *enemy_create(
    enemies_t *enemies, sfVector2f position, int id, int speed)
{
    enemies_t *new;

    new = calloc(1, sizeof(enemies_t));
    if (enemies)
        enemies->prev = new;
    new->next = enemies;
    new->id = id;
    new->position = position;
    new->speed = speed;
    return (new);
}

enemies_t *enemy_pop(enemies_t *array)
{
    enemies_t *tmp = array ? array->next : NULL;

    if (array->prev)
        array->prev->next = array->next;
    if (array->next)
        array->next->prev = array->prev;
    free(array);
    return (tmp);
}

enemies_t *enemies_refresh(
    enemies_t *enemies, particle_arr_t **array, float time)
{
    enemies_t *tmp = enemies;

    while (tmp) {
        if (tmp->position.y >= 472) {
            *array = append_particle(*array,
                create_particles(
                    (particle_request_t){sfWhite, 150, tmp->position, 35, 5, 6,
                        (refresh_function_t[2]){refresh_shrink, NULL}}));
            if (tmp == enemies) {
                enemies = enemy_pop(enemies);
                tmp = enemies;
            } else
                tmp = enemy_pop(tmp);
        } else {
            *array = append_particle(*array,
                create_particles((particle_request_t){
                    (sfColor){.a = 0xFF, .r = 25, .g = 25, .b = 25}, 5,
                    tmp->position, 25, 1, 3,
                    (refresh_function_t[2]){refresh_shrink, NULL}}));
            tmp->position.y += (float)tmp->speed * time;
            tmp = tmp->next;
        }
    }
    return (enemies);
}