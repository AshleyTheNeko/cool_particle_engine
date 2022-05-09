#include "attacks_s.h"

#include <stdlib.h>

void attack_push(attacks_t **attacks, int new_id, int position)
{
    attacks_t *new;

    new = calloc(1, sizeof(attacks_t));
    if (*attacks)
        (*attacks)->prev = new;
    new->next = *attacks;
    new->x_pos = position;
    new->y_pos = 472;
    new->id = new_id;
    *attacks = new;
}

attacks_t *attack_pop(attacks_t *array)
{
    attacks_t *tmp = array ? array->next : NULL;

    if (array->prev)
        array->prev->next = array->next;
    if (array->next)
        array->next->prev = array->prev;
    free(array);
    return (tmp);
}

attacks_t *attacks_refresh(attacks_t *attacks, players_t *players)
{
    attacks_t *tmp = attacks;

    while (tmp) {
        if (tmp->y_pos <= 0) {
            players_broadcast(players, "disp_atk:%d", tmp->id);
            if (tmp == attacks) {
                attacks = attack_pop(attacks);
                tmp = attacks;
            } else
                tmp = attack_pop(tmp);
        } else {
            tmp->y_pos -= 5;
            tmp = tmp->next;
        }
    }
    return (attacks);
}
