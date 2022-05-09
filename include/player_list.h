#pragma once

#include <SFML/Graphics.h>

typedef struct player {
    sfVector2f position;
    int id;
    sfCircleShape *shape;
    struct player *next;
    struct player *prev;
} players_t;

void player_create(players_t **players, sfVector2f position, int id);
void player_pop(players_t **array, int id);
void players_draw(sfRenderWindow *win, players_t *players);
players_t *player_get(players_t *array, int id);
