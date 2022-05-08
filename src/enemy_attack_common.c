#include "attack.h"
#include "enemies.h"
#include <stdlib.h>

void enemy_collide_projectile(enemies_t **enemies, particle_arr_t **particles,
    attacks_t **attacks, int *score)
{
    attacks_t *attack_tmp = *attacks;
    enemies_t *enemy_tmp = *enemies;

    while (attack_tmp) {
        if (!(*enemies))
            break;
        enemy_tmp = *enemies;
        while (enemy_tmp) {
            if (!attack_tmp || !enemy_tmp)
                break;
            if (abs((int)(attack_tmp->position.y - enemy_tmp->position.y)) <
                    15 &&
                abs((int)(attack_tmp->position.x - enemy_tmp->position.x)) <
                    15) {
                (*score) += 100;
                *particles = append_particle(*particles,
                    create_particles((particle_request_t){
                        (sfColor){.a = 0xFF, .r = 227, .g = 103, .b = 9}, 150,
                        attack_tmp->position, 20, 2, 3, NULL}));
                *particles = append_particle(*particles,
                    create_particles((particle_request_t){sfRed, 100,
                        attack_tmp->position, 40, 4, 5,
                        (refresh_function_t[2]){refresh_shrink, NULL}}));
                if (attack_tmp == (*attacks)) {
                    *attacks = attack_pop(*attacks);
                    attack_tmp = *attacks;
                } else
                    attack_tmp = attack_pop(attack_tmp);
                if (enemy_tmp == (*enemies)) {
                    *enemies = enemy_pop(*enemies);
                    enemy_tmp = *enemies;
                } else
                    enemy_tmp = enemy_pop(enemy_tmp);
            } else
                enemy_tmp = enemy_tmp->next;
        }
        if (attack_tmp)
            attack_tmp = attack_tmp->next;
    }
}