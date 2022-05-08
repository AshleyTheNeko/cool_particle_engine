#pragma once

#include "attack.h"
#include "enemies.h"

void enemy_collide_projectile(enemies_t **enemies, particle_arr_t **particles,
    attacks_t **attacks, int *score);
