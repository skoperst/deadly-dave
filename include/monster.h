#ifndef MONSTER_H
#define MONSTER_H

#include "tile.h"
#include "plasma.h"

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
    int ticks_before_shoot;

    // Fire rate is the amount of ticks need to pass from no monster's plasma on screen,
    // until this monster needs to create a plasma.
    int fire_rate;

    int on_fire;
    tile_t *tile;
    plasma_t *plasma;

    void (*tick)(struct monster_struct *monster, int dave_x);
    int (*is_dead)(struct monster_struct *monster);
    int (*is_alive)(struct monster_struct *monster);
} monster_t;

monster_t* monster_create(int x, int y, int w, int h);
monster_t* monster_create_sun(int x, int y);
monster_t* monster_create_spider(int x, int y);
monster_t* monster_create_swirl(int x, int y);
monster_t* monster_create_bones(int x, int y);
monster_t* monster_create_ufo(int x, int y);

#endif
