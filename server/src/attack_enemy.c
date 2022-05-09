#include "attack_enemy.h"
#include <math.h>
#include <stdlib.h>

void enemy_collide_projectile(
    players_t *players, enemies_t **enemies, attacks_t **attacks, int *score)
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
            if (abs((int)(attack_tmp->y_pos - enemy_tmp->y_pos)) < 15 &&
                abs((int)(attack_tmp->x_pos - enemy_tmp->x_pos)) < 15) {
                (*score) += 100;
                players_broadcast(players, "scr:%d", *score);
                players_broadcast(
                    players, "hit:%d:%d", enemy_tmp->id, attack_tmp->id);
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