#include "player_list.h"
#include <stdlib.h>

static const sfColor COLORS[] = {{.a = 0xFF, .r = 0xFF, .g = 0, .b = 0},
    {.a = 0xFF, .r = 0, .g = 0xFF, .b = 0},
    {.a = 0xFF, .r = 0, .g = 0, .b = 0xFF},
    {.a = 0xFF, .r = 0, .g = 0xFF, .b = 0xFF},
    {.a = 0xFF, .r = 0xFF, .g = 0, .b = 0xFF},
    {.a = 0xFF, .r = 0xFF, .g = 0xFF, .b = 0}};

void player_create(players_t **players, sfVector2f position, int id)
{
    players_t *new;

    new = calloc(1, sizeof(players_t));
    if (*players)
        (*players)->prev = new;
    new->next = (*players);
    new->position = position;
    new->id = id;
    new->shape = sfCircleShape_create();
    sfCircleShape_setFillColor(new->shape, COLORS[id % 6]);
    sfCircleShape_setPointCount(new->shape, 3);
    sfCircleShape_setRadius(new->shape, 15.5);
    sfCircleShape_setPosition(new->shape, position);
    *players = new;
}

void player_pop(players_t **array, int id)
{
    for (players_t *tmp = *array; tmp; tmp = tmp->next) {
        if (tmp->id != id)
            continue;
        if (tmp->prev)
            tmp->prev->next = tmp->next;
        if (tmp->next)
            tmp->next->prev = tmp->prev;
        if (tmp == *array)
            *array = tmp->next;
        sfCircleShape_destroy(tmp->shape);
        free(tmp);
        return;
    }
}

players_t *player_get(players_t *array, int id)
{
    for (players_t *tmp = array; tmp; tmp = tmp->next) {
        if (tmp->id == id)
            return (tmp);
    }
    return (NULL);
}

void players_draw(sfRenderWindow *win, players_t *players)
{
    for (players_t *tmp = players; tmp; tmp = tmp->next)
        sfRenderWindow_drawCircleShape(win, tmp->shape, NULL);
}
