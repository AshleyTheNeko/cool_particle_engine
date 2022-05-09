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

static void parse_client_message(
    players_t *players, players_t *from, char *message)
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
        tmp = strtok(NULL, ":");
        players_broadcast(players, "atk:%d", atoi(tmp));
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
    struct timeval tv;
    time_t spawn_clock = time(NULL);
    time_t delta;
    char buffer[4096] = {0};

    FD_ZERO(&sockets);
    safe_fd_set(server, &sockets, &max_fd);
    while (1) {
        sockets_tmp = sockets;

        tv = (struct timeval){1, 0};
        if (select(max_fd + 1, &sockets_tmp, NULL, NULL, &tv) < 0) {
            perror("select");
            return -1;
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
            dprintf(client, "pls");
            for (players_t *tmp = players; tmp; tmp = tmp->next)
                dprintf(client, ":%d:%d", tmp->socket, tmp->position);
            dprintf(client, "\n");
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
                parse_client_message(players, tmp, buffer);
            }
            tmp = tmp->next;
        }
        delta = time(NULL) - spawn_clock;
        if (delta >= 2) {
            spawn_clock = time(NULL);
        }
    }
    return (0);
}