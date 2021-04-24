#ifndef BULLET_H
#define BULLET_H

#include "tile.h"

#define BULLET_STATE_FLYING_RIGHT 1
#define BULLET_STATE_FLYING_LEFT  2
#define BULLET_STATE_DEAD         3

typedef struct bullet_struct {
    tile_t *tile;

    int speed_x;
    int state;
    int steps;

    void (*tick)(struct bullet_struct *bullet, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
        int deadzone_left, int deadzone_right);
    int (*is_dead)(struct bullet_struct *bullet);
} bullet_t;

bullet_t* bullet_create_right(int x, int y);
bullet_t* bullet_create_left(int x, int y);

#endif
