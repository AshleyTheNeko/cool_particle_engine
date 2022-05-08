#include "particle.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

static sfVector2f get_float_pair(float speed)
{
    float random_angle = ((float)rand() / (float)RAND_MAX) * (2.f * M_PI);
    float rand_x =
        cosf(random_angle) * (((float)rand() / (float)RAND_MAX) * speed);
    float rand_y =
        sinf(random_angle) * (((float)rand() / (float)RAND_MAX) * speed);
    return ((sfVector2f){rand_y, rand_x});
}

particle_t *create_particles(particle_request_t to_create)
{
    particle_t *particle = calloc(1, sizeof(particle_t));
    particle->lifetime = to_create.lifetime;
    particle->initial_position = to_create.pos;
    int functions = 0;

    if (!to_create.refresh_functions)
        particle->refresh_functions = NULL;
    else {
        while (to_create.refresh_functions[functions])
            functions++;
        particle->refresh_functions = calloc(functions, sizeof(void *));
        memcpy(particle->refresh_functions, to_create.refresh_functions,
            sizeof(void *) * functions);
    }
    particle->function_count = functions;
    particle->shapes =
        calloc(to_create.amount + 1, sizeof(sfRectangleShape *));
    particle->vectors = calloc(to_create.amount, sizeof(sfVector2f));

    for (int i = 0; i != to_create.amount; i++) {
        particle->shapes[i] = sfRectangleShape_create();
        particle->vectors[i] = get_float_pair(to_create.speed);
        sfRectangleShape_setSize(
            particle->shapes[i], (sfVector2f){to_create.size, to_create.size});
        sfRectangleShape_setFillColor(particle->shapes[i], to_create.color);
        sfRectangleShape_setPosition(particle->shapes[i], to_create.pos);
    }
    return (particle);
}

int refresh_particles(particle_t *particle)
{
    if (!particle)
        return (0);
    particle->life++;
    if (particle->life == particle->lifetime)
        return (1);
    for (int i = 0; particle->shapes[i]; i++) {
        if (particle->function_count) {
            for (size_t funct = 0; funct < particle->function_count; funct++)
                (particle->refresh_functions[funct])(particle, i);
        }
        sfRectangleShape_move(particle->shapes[i], particle->vectors[i]);
    }
    return (0);
}

void display_particles(particle_t *particle, sfRenderWindow *win)
{
    for (int i = 0; particle->shapes[i]; i++)
        sfRenderWindow_drawRectangleShape(win, particle->shapes[i], NULL);
}

void destroy_particles(particle_t *particle)
{
    for (int i = 0; particle->shapes[i]; i++)
        sfRectangleShape_destroy(particle->shapes[i]);
    free(particle->vectors);
    free(particle);
}