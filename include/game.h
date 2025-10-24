#ifndef _GAME_H_
#define _GAME_H_

#include <SDL.h>

#include "tile.h"
#include "dave.h"
#include "bullet.h"
#include "monster.h"
#include "plasma.h"

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
#define MAX_MONSTERS 10
#define MAX_PLASMAS 10


#define G_STATE_NONE              0
#define G_STATE_INTRO             1
#define G_STATE_LEVEL_START       2
#define G_STATE_LEVEL_BLINKING    3
#define G_STATE_LEVEL             4
#define G_STATE_LEVEL_POPUP       5
#define G_STATE_WARP_START        6
#define G_STATE_WARP              7
#define G_STATE_WARP_POPUP        8
#define G_STATE_GAMEOVER          9
#define G_STATE_QUIT_NOW          10

#define SECRET_LEVEL_NOT_VISITED 0
#define SECRET_LEVEL_ENTER 1
#define SECRET_LEVEL_VISITED 2

#define WARP_NONE  0
#define WARP_RIGHT 1
#define WARP_DOWN  2

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
    uint8_t tick;
    uint8_t dave_tick;
    uint8_t dave_dead_timer;
    uint8_t blinking_timer;

    dave_t *dave;
    bullet_t *bullet;
    plasma_t *plasmas[MAX_PLASMAS];
    monster_t *monsters[MAX_MONSTERS];
    tile_t flashing_cursor;
    tile_t bottom_separator;
    tile_t top_separator;
    tile_t grail_banner;
    tile_t gun_banner;

    int64_t scroll_offset;
    int64_t scroll_remaining;

    uint64_t level;

    uint64_t level_secret_state;
    uint64_t lives;
    uint64_t score;

    int32_t in_warp;
} game_context_t;


typedef struct game_assets {
    SDL_Surface *imgdata[1000];
} assets_t;

int game_main(int is_windowed, int starting_level);
#endif
