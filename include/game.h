#ifndef _GAME_H_
#define _GAME_H_

#include <SDL.h>

#include "tile.h"
#include "dave.h"
#include "bullet.h"
#include "monster.h"

#define RIGHT 50
#define LEFT  51
#define UP    52
#define DOWN  53

#define DAVE_WALKING_STATE_STANDING  0
#define DAVE_WALKING_STATE_COOLDOWN1_RIGHT  1
#define DAVE_WALKING_STATE_COOLDOWN1_LEFT  2
#define DAVE_WALKING_STATE_COOLDOWN2_RIGHT  3
#define DAVE_WALKING_STATE_COOLDOWN2_LEFT  4
#define DAVE_WALKING_STATE_RIGHT 5
#define DAVE_WALKING_STATE_LEFT 6

typedef struct keys_state_struct {
    int32_t jump;
    int32_t left;
    int32_t right;
    int32_t down;
    int32_t fire;
    int32_t jetpack;
    int32_t quit;
    int32_t bracer;
    int32_t bracel;
    int32_t escape;
    int32_t enter;
    int32_t space;
    int32_t key_y;
    int32_t key_n;
} keys_state_t;

typedef struct game_context_struct {
    uint8_t quit;
    uint8_t tick;
    uint8_t dave_tick;
    uint8_t dave_dead_timer;
    uint16_t dbullet_px;
    uint16_t dbullet_py;
    int8_t dbullet_dir;
    uint16_t ebullet_px;
    uint16_t ebullet_py;
    int8_t ebullet_dir;
    int16_t dave_px;
    int16_t dave_py;
    uint8_t on_ground;
    int8_t scroll_x;
    int8_t last_dir;

    uint8_t dave_right;
    uint8_t dave_left;
    uint8_t dave_jump;
    uint8_t dave_fire;
    uint8_t dave_down;
    uint8_t dave_up;
    uint8_t dave_climb;
    uint8_t dave_jetpack;
    uint8_t jetpack_delay;
    uint8_t jump_timer;
    uint8_t try_right;
    uint8_t gun;
    uint8_t jetpack;

    dave_t *dave;
    bullet_t *bullet;
    monster_t *monsters[5];

    int64_t scroll_offset;
    int64_t scroll_remaining;

    uint64_t level;
    uint64_t lives;
    uint64_t score;
} game_context_t;


typedef struct game_assets {
    SDL_Surface *tiles[1000];
} assets_t;

int game_main(int is_windowed, int starting_level);
#endif
