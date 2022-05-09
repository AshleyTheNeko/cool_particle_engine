#include "enemies_s.h"

#include <stdlib.h>

void enemy_push(enemies_t **enemies, int new_id, int position, int speed)
{
    enemies_t *new;

    new = calloc(1, sizeof(enemies_t));
    if (*enemies)
        (*enemies)->prev = new;
    new->next = *enemies;
    new->x_pos = position;
    new->speed = speed;
    new->y_pos = 0;
    new->id = new_id;
    *enemies = new;
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

enemies_t *enemies_refresh(enemies_t *enemies, players_t *players, int *score)
{
    enemies_t *tmp = enemies;

    while (tmp) {
        if (tmp->y_pos >= 472) {
            (*score) -= 1000;
            players_broadcast(players, "scr:%d", *score);
            players_broadcast(players, "enm_tch:%d", tmp->id);
            if (tmp == enemies) {
                enemies = enemy_pop(enemies);
                tmp = enemies;
            } else
                tmp = enemy_pop(tmp);
        } else {
            tmp->y_pos += tmp->speed;
            tmp = tmp->next;
        }
    }
    return (enemies);
}