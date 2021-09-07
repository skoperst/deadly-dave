#ifndef DAVE_H
#define DAVE_H

#include "tile.h"
#include "soundfx.h"

#define DAVE_WALKING_STATE_STANDING  0
#define DAVE_WALKING_STATE_COOLDOWN1_RIGHT  1
#define DAVE_WALKING_STATE_COOLDOWN1_LEFT  2
#define DAVE_WALKING_STATE_COOLDOWN2_RIGHT  3
#define DAVE_WALKING_STATE_COOLDOWN2_LEFT  4
#define DAVE_WALKING_STATE_RIGHT 5
#define DAVE_WALKING_STATE_LEFT 6

#define DAVE_CLIMBING_STATE_READY 0
#define DAVE_CLIMBING_STATE_COOLDOWN 1
#define DAVE_CLIMBING_STATE_JUMP_RIGHT 2
#define DAVE_CLIMBING_STATE_JUMP_LEFT 3


#define DAVE_STATE_STANDING    0
#define DAVE_STATE_WALKING     1
#define DAVE_STATE_JUMPING     2
#define DAVE_STATE_CLIMBING    3
#define DAVE_STATE_FREEFALLING 4
#define DAVE_STATE_JETPACKING  5
#define DAVE_STATE_BURNING     6
#define DAVE_STATE_DEAD        7
#define DAVE_STATE_BLINKING    8

#define DAVE_DIRECTION_FRONTR  0
#define DAVE_DIRECTION_FRONTL  1
#define DAVE_DIRECTION_LEFT    2
#define DAVE_DIRECTION_RIGHT   3

typedef struct dave_struct {
    tile_t *tile;

    soundfx_t *sfx;
    int state;
    int face_direction;
    int has_trophy;
    int has_gun;
    int has_jetpack;
    int ticks_in_state;
    int mute;

    // sub-states & helpers specific to states
    int walk_state;
    int step_count;
    int jump_state;
    int jump_cooldown_count;
    int freefall_direction;
    int climb_state;

    void (*tick)(struct dave_struct *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
        int left, int right, int up, int down, int jetpack);
    int (*is_dead)(struct dave_struct *dave);
} dave_t;

dave_t* dave_create(soundfx_t *sfx);

#endif
