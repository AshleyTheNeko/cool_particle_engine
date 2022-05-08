#include "attack.h"
#include <stdlib.h>

attacks_t *attack_create(attacks_t *attacks, sfVector2f position)
{
    attacks_t *new;

    new = calloc(1, sizeof(attacks_t));
    if (attacks)
        attacks->prev = new;
    new->next = attacks;
    new->position = position;
    return (new);
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

attacks_t *attacks_refresh(attacks_t *attacks, particle_arr_t **array)
{
    attacks_t *tmp = attacks;

    while (tmp) {
        if (tmp->position.y <= 0) {
            if (tmp == attacks) {
                attacks = attack_pop(attacks);
                tmp = attacks;
            } else
                tmp = attack_pop(tmp);
        } else {
            *array = append_particle(*array,
                create_particles((particle_request_t){
                    (sfColor){.a = 0xFF, .r = 155, .g = 155, .b = 155}, 5,
                    tmp->position, 4, 2, 3, NULL}));
            tmp->position.y -= 5;
            tmp = tmp->next;
        }
    }
    return (attacks);
}