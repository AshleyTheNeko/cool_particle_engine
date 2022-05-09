#pragma once
#include "attacks_s.h"
#include "enemies_s.h"
#include "player.h"

void enemy_collide_projectile(
    players_t *players, enemies_t **enemies, attacks_t **attacks, int *score);
