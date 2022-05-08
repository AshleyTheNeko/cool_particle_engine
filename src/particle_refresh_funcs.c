#include "particle.h"

void refresh_fade(particle_t *particle, int offset)
{
    (void)particle;
    (void)offset;
}

void refresh_shrink(particle_t *particle, int offset)
{
    sfVector2f size;

    if (((float)particle->life / (float)particle->lifetime) > 0.7) {
        size = sfRectangleShape_getSize(particle->shapes[offset]);
        size.x /= 1.2;
        size.y /= 1.2;
        sfRectangleShape_setSize(particle->shapes[offset], size);
    }
}

void refresh_slow(particle_t *particle, int offset)
{
    if (((float)particle->life / (float)particle->lifetime) > 0.6) {
        particle->vectors[offset].x /= 1.1;
        particle->vectors[offset].y /= 1.1;
    }
}