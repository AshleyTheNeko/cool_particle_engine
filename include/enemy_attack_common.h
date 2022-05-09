#pragma once

#include "attack.h"
#include "enemies.h"

void trigger_hit(enemies_t **enemies, particle_arr_t **particles,
    attacks_t **attacks, int ids[2]);
