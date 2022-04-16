#ifndef MONSTER_H
#define MONSTER_H

#include "tile.h"
#include "soundfx.h"

#define MONSTER_STATE_UNINITIALIZED        0
#define MONSTER_STATE_STANDBY              1
#define MONSTER_STATE_ACTIVE               2
#define MONSTER_STATE_BURNING              3
#define MONSTER_STATE_DEAD                 4

typedef struct monster_struct {
    int state;
    int route[256];
    int route_sz;
    int route_idx;
    int cooldown;
    tile_t *tile;

    void (*tick)(struct monster_struct *monster);
    int (*is_dead)(struct monster_struct *monster);
} monster_t;

monster_t* monster_create();
monster_t* monster_create_sun();
monster_t* monster_create_spider();

#endif
