#pragma once

#include <SFML/Graphics.h>

typedef struct particle particle_t;

typedef void (*refresh_function_t)(particle_t *particle, int offset);

struct particle {
    sfRectangleShape **shapes;
    sfVector2f *vectors;
    int lifetime;
    int life;
    sfVector2f initial_position;
    refresh_function_t *refresh_functions;
    size_t function_count;
};

typedef struct {
    sfColor color;
    int amount;
    sfVector2f pos;
    int lifetime;
    float speed;
    int size;
    refresh_function_t *refresh_functions;
} particle_request_t;

particle_t *create_particles(particle_request_t to_create);
int refresh_particles(particle_t *particle);
void display_particles(particle_t *particle, sfRenderWindow *win);
void destroy_particles(particle_t *particle);

void refresh_fade(particle_t *particle, int offset);
void refresh_shrink(particle_t *particle, int offset);
void refresh_slow(particle_t *particle, int offset);
