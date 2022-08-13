#ifndef MONSTER_H
#define MONSTER_H

#include "tile.h"
#include "soundfx.h"

#define MONSTER_STATE_BLINKING             0
#define MONSTER_STATE_ACTIVE               1
#define MONSTER_STATE_BURNING              2
#define MONSTER_STATE_DEAD                 3

typedef struct monster_struct {
    int state;
    int route[256];
    int route_sz;
    int route_idx;
    int cooldown;
    int ticks_in_state;
    int on_fire;
    tile_t *tile;

    void (*tick)(struct monster_struct *monster);
    int (*is_dead)(struct monster_struct *monster);
    int (*is_alive)(struct monster_struct *monster);
    int (*is_shooting)(struct monster_struct *monster, int off, int tot);
} monster_t;

monster_t* monster_create();
monster_t* monster_create_sun();
monster_t* monster_create_spider();
monster_t* monster_create_swirl();
monster_t* monster_create_bones();

#endif
