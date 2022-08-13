#ifndef PLASMA_H
#define PLASMA_H

#include "tile.h"

#define PLASMA_STATE_FLYING_RIGHT 1
#define PLASMA_STATE_FLYING_LEFT  2
#define PLASMA_STATE_DEAD         3

typedef struct plasma_struct {
    tile_t *tile;

    int speed_x;
    int state;
    int steps;

    void (*tick)(struct plasma_struct *plasma, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
        int deadzone_left, int deadzone_right);
    int (*is_dead)(struct plasma_struct *plasma);
    int (*get_sprite)(struct plasma_struct *plasma);
} plasma_t;

plasma_t* plasma_create_right(int x, int y);
plasma_t* plasma_create_left(int x, int y);

#endif
