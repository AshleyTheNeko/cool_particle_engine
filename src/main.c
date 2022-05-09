#include "client.h"
#include "enemy_attack_common.h"
#include "player_list.h"
#include <SFML/Graphics.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc != 3)
        return (1);
    int sock = connect_to_server(argv[1], atoi(argv[2]));
    if (sock == -1)
        return (1);

    sfRenderWindow *win = sfRenderWindow_create(
        (sfVideoMode){.bitsPerPixel = 32, .height = 500, .width = 500},
        "Particle invaders", sfTitlebar | sfClose, NULL);
    sfEvent evt;
    sfClock *animation_clock = sfClock_create();
    sfClock *fire_clock = sfClock_create();
    sfClock *particle_clock = sfClock_create();
    sfClock *move_clock = sfClock_create();
    particle_arr_t *particles = NULL;
    sfView *view = sfView_copy(sfRenderWindow_getView(win));
    enemies_t *enemies = NULL;
    attacks_t *attacks = NULL;
    sfText *score_text = sfText_create();
    sfFont *pixel_font = sfFont_createFromFile("./assets/Minecraft.ttf");
    int score = 0;
    char score_string[128] = {0};
    players_t *players = NULL;
    fd_set server_set;
    fd_set server_set_backup;
    struct timeval tv;

    FD_ZERO(&server_set);
    FD_SET(sock, &server_set);
    sfText_setFillColor(score_text, sfWhite);
    sfText_setFont(score_text, pixel_font);
    sfText_setPosition(score_text, (sfVector2f){15, 15});
    sfText_setString(score_text, score_string);
    sprintf(score_string, "%d", score);
    sfRenderWindow_setFramerateLimit(win, 144);
    sfRenderWindow_setView(win, view);
    srand(time(NULL));
    while (1) {
        while (sfRenderWindow_pollEvent(win, &evt)) {
            if (evt.type == sfEvtKeyPressed && evt.key.code == sfKeyEscape) {
                sfRenderWindow_destroy(win);
                return (0);
            }
            if (evt.type == sfEvtClosed) {
                sfRenderWindow_destroy(win);
                return (0);
            }
        }
        if (sfTime_asMilliseconds(sfClock_getElapsedTime(move_clock)) > 10) {
            if (sfRenderWindow_hasFocus(win) &&
                (sfKeyboard_isKeyPressed(sfKeyLeft) ||
                    sfKeyboard_isKeyPressed(sfKeyQ)))
                write(sock, "mv:0\n", 5);
            if (sfRenderWindow_hasFocus(win) &&
                (sfKeyboard_isKeyPressed(sfKeyRight) ||
                    sfKeyboard_isKeyPressed(sfKeyD)))
                write(sock, "mv:1\n", 5);
            sfClock_restart(move_clock);
        }
        while (1) {
            server_set_backup = server_set;
            tv = (struct timeval){0, 0};
            if (select(sock + 1, &server_set_backup, NULL, NULL, &tv) < 0) {
                perror("select");
                sfRenderWindow_destroy(win);
                return (1);
            }
            if (FD_ISSET(sock, &server_set_backup)) {
                if (get_server_command(sock, &players, &particles, &score,
                        &attacks, &enemies)) {
                    printf("Connection lost.\n");
                    sfRenderWindow_destroy(win);
                    return (1);
                }
            } else
                break;
        }
        if (sfTime_asMilliseconds(sfClock_getElapsedTime(fire_clock)) > 500) {
            if (sfRenderWindow_hasFocus(win) &&
                sfKeyboard_isKeyPressed(sfKeySpace)) {
                write(sock, "atk\n", 4);
                sfClock_restart(fire_clock);
            }
        }
        if (sfTime_asMilliseconds(sfClock_getElapsedTime(animation_clock)) >=
            10) {
            if (attacks)
                attacks = attacks_refresh(attacks, &particles,
                    (float)sfTime_asMilliseconds(
                        sfClock_getElapsedTime(animation_clock)) /
                        25.f);
            if (enemies)
                enemies = enemies_refresh(enemies, &particles,
                    (float)sfTime_asMilliseconds(
                        sfClock_getElapsedTime(animation_clock)) /
                        25.f);
            sprintf(score_string, "SCORE: %d", score);
            sfText_setString(score_text, score_string);
            sfClock_restart(animation_clock);
        }
        if (sfTime_asMilliseconds(sfClock_getElapsedTime(particle_clock)) >=
            25) {
            if (particles)
                particles = refresh_particle_array(particles);
            sfClock_restart(particle_clock);
        }
        sfRenderWindow_clear(
            win, (sfColor){.a = 0xFF, .r = 80, .g = 80, .b = 80});
        if (particles)
            display_particle_array(win, particles);
        players_draw(win, players);
        sfRenderWindow_drawText(win, score_text, NULL);
        sfRenderWindow_display(win);
    }
    return (0);
}
