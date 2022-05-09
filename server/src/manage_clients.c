#include "attack_enemy.h"
#include "player.h"
#include "server.h"
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <time.h>
#include <unistd.h>

int safe_fd_set(int fd, fd_set *fds, int *max_fd)
{
    FD_SET(fd, fds);
    if (fd > *max_fd)
        *max_fd = fd;
    return 0;
}

int safe_fd_clr(int fd, fd_set *fds, int *max_fd)
{
    FD_CLR(fd, fds);
    if (fd == *max_fd)
        (*max_fd)--;
    return 0;
}

static void parse_client_message(players_t *players, players_t *from,
    char *message, attacks_t **attacks, int *id_giver)
{
    char *tmp;

    tmp = strtok(message, ":");
    if (!strcmp(tmp, "mv")) {
        tmp = strtok(NULL, ":");
        if (atoi(tmp) == 1) {
            if (from->position + 3 > 473)
                return;
            from->position += 3;
        } else {
            if (from->position - 3 < 0)
                return;
            from->position -= 3;
        }
        players_broadcast(players, "mv:%d:%d", from->socket, from->position);
        return;
    }
    if (!strcmp(tmp, "atk")) {
        *id_giver = ((*id_giver) + 1) % 999;
        attack_push(attacks, *id_giver, from->position + 13);
        players_broadcast(
            players, "atk:%d:%d", *id_giver, from->position + 13);
        return;
    }
}

int listen_clients(file_descriptor_t server)
{
    file_descriptor_t client = 0;
    struct sockaddr_in client_settings;
    fd_set sockets;
    fd_set sockets_tmp;
    file_descriptor_t tmp_socket;
    int max_fd;
    players_t *players = NULL;
    enemies_t *enemies = NULL;
    attacks_t *attacks = NULL;
    struct timeval tv;
    struct timespec move_clock;
    struct timespec spawn_clock;
    struct timespec refresh;
    unsigned long int delta_ms;
    int score = 0;
    char buffer[4096] = {0};
    int speed;
    int position;
    int id_giver = 0;
    int delay = 4000;

    srand(time(NULL));
    clock_gettime(CLOCK_MONOTONIC_RAW, &move_clock);
    clock_gettime(CLOCK_MONOTONIC_RAW, &spawn_clock);
    FD_ZERO(&sockets);
    safe_fd_set(server, &sockets, &max_fd);
    while (1) {
        sockets_tmp = sockets;

        tv = (struct timeval){0, 5000};
        if (select(max_fd + 1, &sockets_tmp, NULL, NULL, &tv) < 0) {
            perror("select");
            return (-1);
        }
        if (FD_ISSET(server, &sockets_tmp)) {
            client = accept(server, (struct sockaddr *)&client_settings,
                (socklen_t[1]){sizeof(struct sockaddr_in)});
            if (client == -1) {
                perror("");
                return (-1);
            }
            players_broadcast(players, "con:%d", client);
            printf("Player %d connect.\n", client);
            safe_fd_set(client, &sockets, &max_fd);
            player_push(&players, client);
            dprintf(client, "scr:%d\n", score);
            dprintf(client, "pls");
            for (players_t *tmp = players; tmp; tmp = tmp->next)
                dprintf(client, ":%d:%d", tmp->socket, tmp->position);
            dprintf(client, "\n");
            if (attacks) {
                dprintf(client, "atks");
                for (attacks_t *tmp = attacks; tmp; tmp = tmp->next)
                    dprintf(
                        client, ":%d:%d:%d", tmp->id, tmp->x_pos, tmp->y_pos);
                dprintf(client, "\n");
            }
            if (enemies) {
                dprintf(client, "enys");
                for (enemies_t *tmp = enemies; tmp; tmp = tmp->next)
                    dprintf(client, ":%d:%d:%d:%d", tmp->id, tmp->x_pos,
                        tmp->y_pos, tmp->speed);
                dprintf(client, "\n");
            }
        }
        for (players_t *tmp = players; tmp;) {
            if (FD_ISSET(tmp->socket, &sockets_tmp)) {
                if (get_socket_line(tmp->socket, buffer, 4096) < 1) {
                    printf("Player %d disconnect.\n", tmp->socket);
                    tmp_socket = tmp->socket;
                    close(tmp->socket);
                    safe_fd_clr(tmp->socket, &sockets, &max_fd);
                    if (tmp == players) {
                        players = player_pop(tmp);
                        tmp = players;
                    } else
                        tmp = player_pop(tmp);
                    players_broadcast(players, "dis:%d", tmp_socket);
                    continue;
                }
                parse_client_message(
                    players, tmp, buffer, &attacks, &id_giver);
            }
            tmp = tmp->next;
        }
        clock_gettime(CLOCK_MONOTONIC_RAW, &refresh);
        delta_ms = (refresh.tv_sec - spawn_clock.tv_sec) * 1000 +
            (refresh.tv_nsec - spawn_clock.tv_nsec) / 1000000;
        if ((delta_ms >= delay) && players) {
            position = (rand() % 450) + 25;
            speed = (rand() % 2) + 2;
            id_giver = (id_giver + 1) % 999;
            enemy_push(&enemies, id_giver, position, speed);
            players_broadcast(
                players, "spwn:%d:%d:%d", id_giver, position, speed);
            if (delay > 700)
                delay -= 5;
            clock_gettime(CLOCK_MONOTONIC_RAW, &spawn_clock);
        }
        clock_gettime(CLOCK_MONOTONIC_RAW, &refresh);
        delta_ms = (refresh.tv_sec - move_clock.tv_sec) * 1000 +
            (refresh.tv_nsec - move_clock.tv_nsec) / 1000000;
        if (delta_ms >= 25) {
            if (attacks)
                attacks = attacks_refresh(attacks, players);
            if (enemies)
                enemies = enemies_refresh(enemies, players, &score);
            if (attacks && enemies)
                enemy_collide_projectile(players, &enemies, &attacks, &score);
            clock_gettime(CLOCK_MONOTONIC_RAW, &move_clock);
        }
    }
    return (0);
}