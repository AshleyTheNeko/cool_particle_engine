#include "enemies.h"
#include <stdlib.h>

enemies_t *enemy_create(enemies_t *enemies, sfVector2f position)
{
    enemies_t *new;

    new = calloc(1, sizeof(enemies_t));
    if (enemies)
        enemies->prev = new;
    new->next = enemies;
    new->position = position;
    new->speed = (rand() % 4) + 1;
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
    enemies_t *enemies, particle_arr_t **array, int screen_height, int *score)
{
    enemies_t *tmp = enemies;

    while (tmp) {
        if (tmp->position.y >= screen_height) {
            *array = append_particle(*array,
                create_particles(
                    (particle_request_t){sfWhite, 150, tmp->position, 35, 5, 6,
                        (refresh_function_t[2]){refresh_shrink, NULL}}));
            (*score) -= 1000;
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
            tmp->position.y += tmp->speed;
            tmp = tmp->next;
        }
    }
    return (enemies);
}