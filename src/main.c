#include "enemy_attack_common.h"
#include <SFML/Graphics.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(void)
{
    sfRenderWindow *win = sfRenderWindow_create(
        (sfVideoMode){.bitsPerPixel = 32, .height = 500, .width = 500},
        "Particle invaders", sfDefaultStyle, NULL);
    sfEvent evt;
    sfClock *animation_clock = sfClock_create();
    sfClock *fire_clock = sfClock_create();
    sfClock *spawn_clock = sfClock_create();
    sfClock *move_clock = sfClock_create();
    particle_arr_t *particles = NULL;
    sfView *view = sfView_copy(sfRenderWindow_getView(win));
    sfCircleShape *player = sfCircleShape_create();
    sfVector2f player_pos = {0, 0};
    enemies_t *enemies = NULL;
    attacks_t *attacks = NULL;
    sfText *score_text = sfText_create();
    sfFont *pixel_font = sfFont_createFromFile("./assets/Minecraft.ttf");
    int score = 0;
    char score_string[128] = {0};

    sfText_setFillColor(score_text, sfWhite);
    sfText_setFont(score_text, pixel_font);
    sfText_setPosition(score_text, (sfVector2f){15, 15});
    sfText_setString(score_text, score_string);
    sprintf(score_string, "%d", score);
    sfCircleShape_setFillColor(player, sfWhite);
    sfCircleShape_setPointCount(player, 3);
    sfCircleShape_setRadius(player, 15.5);
    sfRenderWindow_setFramerateLimit(win, 144);
    sfRenderWindow_setView(win, view);
    srand(time(NULL));
    while (1) {
        while (sfRenderWindow_pollEvent(win, &evt)) {
            if (evt.type == sfEvtKeyPressed && evt.key.code == sfKeyEscape) {
                sfRenderWindow_destroy(win);
                return (0);
            }
            if (evt.type == sfEvtResized) {
                player_pos.y = evt.size.height -
                    sfCircleShape_getGlobalBounds(player).height - 5;
                sfCircleShape_setPosition(player, player_pos);
                sfView_reset(view,
                    (sfFloatRect){0, 0, evt.size.width, evt.size.height});
                sfRenderWindow_setView(win, view);
            }
            if (evt.type == sfEvtClosed) {
                sfRenderWindow_destroy(win);
                return (0);
            }
        }
        if (sfTime_asMilliseconds(sfClock_getElapsedTime(move_clock)) > 10) {
            if ((sfKeyboard_isKeyPressed(sfKeyLeft) ||
                    sfKeyboard_isKeyPressed(sfKeyQ)) &&
                player_pos.x >= 5.f) {
                player_pos.x -= 3;
                sfCircleShape_setPosition(player, player_pos);
            }
            if ((sfKeyboard_isKeyPressed(sfKeyRight) ||
                    sfKeyboard_isKeyPressed(sfKeyD)) &&
                (player_pos.x + sfCircleShape_getGlobalBounds(player).width) +
                        5 <=
                    (int)sfView_getSize(sfRenderWindow_getView(win)).x) {
                player_pos.x += 3;
                sfCircleShape_setPosition(player, player_pos);
            }
            sfClock_restart(move_clock);
        }
        if (sfTime_asMilliseconds(sfClock_getElapsedTime(fire_clock)) > 500) {
            if (sfKeyboard_isKeyPressed(sfKeySpace)) {
                attacks = attack_create(attacks,
                    (sfVector2f){player_pos.x +
                            (sfCircleShape_getGlobalBounds(player).width / 2),
                        player_pos.y});
                sfClock_restart(fire_clock);
            }
        }
        if (sfTime_asMilliseconds(sfClock_getElapsedTime(spawn_clock)) >
            2000) {
            enemies = enemy_create(enemies,
                (sfVector2f){
                    (rand() %
                        ((int)sfView_getSize(sfRenderWindow_getView(win)).x -
                            25)) +
                        25,
                    10});
            sfClock_restart(spawn_clock);
        }
        if (sfTime_asMilliseconds(sfClock_getElapsedTime(animation_clock)) >
            25) {
            if (attacks && enemies)
                enemy_collide_projectile(
                    &enemies, &particles, &attacks, &score);
            if (attacks)
                attacks = attacks_refresh(attacks, &particles);
            if (enemies)
                enemies =
                    enemies_refresh(enemies, &particles, player_pos.y, &score);
            if (particles)
                particles = refresh_particle_array(particles);
            sprintf(score_string, "SCORE: %d", score);
            sfText_setString(score_text, score_string);
            sfClock_restart(animation_clock);
        }
        sfRenderWindow_clear(
            win, (sfColor){.a = 0xFF, .r = 80, .g = 80, .b = 80});
        if (particles)
            display_particle_array(win, particles);
        sfRenderWindow_drawCircleShape(win, player, NULL);
        sfRenderWindow_drawText(win, score_text, NULL);
        sfRenderWindow_display(win);
    }
    return (0);
}
