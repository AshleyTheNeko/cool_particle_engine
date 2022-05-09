#include "client.h"
#include "enemy_attack_common.h"
#include "player_list.h"
#include <SFML/Graphics.h>
#include <arpa/inet.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

int connect_to_server(char *address, int port)
{
    int sock;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return (-1);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, address, &serv_addr.sin_addr) <= 0) {
        perror("address");
        return (-1);
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        return (-1);
    }
    return (sock);
}

int get_server_command(int socket, players_t **players,
    particle_arr_t **particles, int *score, attacks_t **attacks,
    enemies_t **enemies)
{
    char *tmp;
    char buffer[2048] = {0};

    if (get_socket_line(socket, buffer, 2048) < 1)
        return (1);
    tmp = strtok(buffer, ":");
    if (!strcmp(tmp, "mv")) {
        tmp = strtok(NULL, ":");
        players_t *player = player_get(*players, atoi(tmp));
        if (!player)
            return (0);
        tmp = strtok(NULL, ":");
        player->position.x = (float)atoi(tmp);
        sfCircleShape_setPosition(player->shape, player->position);
        return (0);
    }
    if (!strcmp(tmp, "pls")) {
        int id;
        int pos;

        while ((tmp = strtok(NULL, ":"))) {
            id = atoi(tmp);
            tmp = strtok(NULL, ":");
            if (!tmp)
                return (0);
            pos = atoi(tmp);
            player_create(players, (sfVector2f){pos, 472}, id);
        }
        return (0);
    }
    if (!strcmp(tmp, "enys")) {
        int id;
        int pos_x;
        int pos_y;
        int speed;

        while ((tmp = strtok(NULL, ":"))) {
            id = atoi(tmp);
            tmp = strtok(NULL, ":");
            if (!tmp)
                return (0);
            pos_x = atoi(tmp);
            tmp = strtok(NULL, ":");
            if (!tmp)
                return (0);
            pos_y = atoi(tmp);
            tmp = strtok(NULL, ":");
            if (!tmp)
                return (0);
            speed = atoi(tmp);
            *enemies =
                enemy_create(*enemies, (sfVector2f){pos_x, pos_y}, id, speed);
        }
        return (0);
    }
    if (!strcmp(tmp, "atks")) {
        int id;
        int pos_x;
        int pos_y;

        while ((tmp = strtok(NULL, ":"))) {
            id = atoi(tmp);
            tmp = strtok(NULL, ":");
            if (!tmp)
                return (0);
            pos_x = atoi(tmp);
            tmp = strtok(NULL, ":");
            if (!tmp)
                return (0);
            pos_y = atoi(tmp);
            *attacks = attack_create(*attacks, (sfVector2f){pos_x, pos_y}, id);
        }
        return (0);
    }
    if (!strcmp(tmp, "con")) {
        tmp = strtok(NULL, ":");
        if (!tmp)
            return (0);
        player_create(players, (sfVector2f){25, 472}, atoi(tmp));
        return (0);
    }
    if (!strcmp(tmp, "scr")) {
        tmp = strtok(NULL, ":");
        if (!tmp)
            return (0);
        *score = atoi(tmp);
        return (0);
    }
    if (!strcmp(tmp, "dis")) {
        tmp = strtok(NULL, ":");
        if (!tmp)
            return (0);
        player_pop(players, atoi(tmp));
        return (0);
    }
    if (!strcmp(tmp, "spwn")) {
        int id;
        int position;
        int speed;

        tmp = strtok(NULL, ":");
        if (!tmp)
            return (0);
        id = atoi(tmp);
        tmp = strtok(NULL, ":");
        if (!tmp)
            return (0);
        position = atoi(tmp);
        tmp = strtok(NULL, ":");
        if (!tmp)
            return (0);
        speed = atoi(tmp);
        *enemies =
            enemy_create(*enemies, (sfVector2f){position, 0}, id, speed);
        return (0);
    }
    if (!strcmp(tmp, "hit")) {
        int atk;
        int enemy;

        tmp = strtok(NULL, ":");
        if (!tmp)
            return (0);
        enemy = atoi(tmp);
        tmp = strtok(NULL, ":");
        if (!tmp)
            return (0);
        atk = atoi(tmp);
        trigger_hit(enemies, particles, attacks, (int[2]){atk, enemy});
        return (0);
    }
    if (!strcmp(tmp, "atk")) {
        int id;
        int position;

        tmp = strtok(NULL, ":");
        if (!tmp)
            return (0);
        id = atoi(tmp);
        tmp = strtok(NULL, ":");
        if (!tmp)
            return (0);
        position = atoi(tmp);
        *attacks = attack_create(*attacks, (sfVector2f){position, 472}, id);
        return (0);
    }
    if (!strcmp(tmp, "dis")) {
        tmp = strtok(NULL, ":");
        if (!tmp)
            return (0);
        player_pop(players, atoi(tmp));
        return (0);
    }
    return (0);
}