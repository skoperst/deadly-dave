#ifndef BULLET_H
#define BULLET_H

#include "tile.h"


#define DAVE_BULLET_DIR_RIGHT  0
#define DAVE_BULLET_DIR_LEFT  1

typedef struct dave_bullet_struct {
    tile_t *tile;

    int direction;
    int speed_x;
    int valid;

    void (*tick)(struct dave_bullet_struct *bullet,
            tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT], int left, int right, int up);
} dave_bullet_t;

dave_bullet_t* dave_bullet_create();

#endif
