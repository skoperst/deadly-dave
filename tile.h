#ifndef BADDAVE_TILE_H
#define BADDAVE_TILE_H

#include <stdio.h>
#include <stdint.h>

/* Quick conversion between grid and pixel basis */
#define TILE_SIZE 16
#define TILEMAP_WIDTH 100
#define TILEMAP_HEIGHT 12
#define TILEMAP_SCENE_X 0
#define TILEMAP_SCENE_Y 0
#define TILEMAP_SCENE_WIDTH 20
#define TILEMAP_SCENE_HEIGHT 12

static const int TILE_DAVE_MOD_FRONT     = 1;
static const int TILE_DAVE_MOD_RIGHT     = 2;
static const int TILE_DAVE_MOD_LEFT      = 3;
static const int TILE_DAVE_MOD_JUMPING_RIGHT = 4;
static const int TILE_DAVE_MOD_JUMPING_LEFT  = 5;

typedef struct tile_dave_struct {
    int x;
    int y;
    int width;
    int height;
    int mod;
    int walk_cycle_offset;
    int cooldown_remaining; //should be out

    int sprites_right[9];
    int sprites_left[9];
    int sprite_jump_right;
    int sprite_jump_left;
    int sprite_front;
    int sprite_idx;

    int (*get_sprite)(struct tile_dave_struct* dave);
    void (*tick)(struct tile_dave_struct* dave, int new_mod);
} tile_dave_t;

typedef struct tile_struct {
    int x;
    int y;
    int width;
    int height;
    int mod;
    int sprites[100];
    int sprite_idx;

    int (*get_sprite)(struct tile_struct* tile);
    void (*tick)(struct tile_struct* tile);
    int (*is_inside)(struct tile_struct* tile, int x, int y);
} tile_t;

// Create simple non-interacting block
void tile_create_block(tile_t* t, int sprite, int x, int y, int width, int height);

// Intro related
void tile_create_intro_fire(tile_t* t, int x, int y);
void tile_create_intro_banner(tile_t* t, int x, int y);

// Popup box
void tile_create_popup_part(tile_t* t, int x, int y);
void tile_create_flashing_cursor(tile_t* t, int x, int y);

// Game screen top & bottom bars
void tile_create_bottom_separator(tile_t* t, int x, int y);

int tile_file_parse(tile_t* map, const char* path);
int tile_map_parse(tile_t* map, char* map_str);

// TILE MODIFIER
static const int CLEAR     = 1;
static const int BRICK     = 2;
static const int ITEM      = 3;


// SPRITE INDEXES
static const int SPRITE_IDX_DOOR       = 2;
static const int SPRITE_IDX_FIRE1      = 6;
static const int SPRITE_IDX_FIRE2      = 7;
static const int SPRITE_IDX_FIRE3      = 8;
static const int SPRITE_IDX_FIRE4      = 9;
static const int SPRITE_IDX_PIPE_RIGHT = 15;
static const int SPRITE_IDX_RED_BRICK  = 17;
static const int SPRITE_IDX_DIRT       = 18;
static const int SPRITE_IDX_CROWN                  = 50;
static const int SPRITE_IDX_CURSOR1                = 167;
static const int SPRITE_IDX_CURSOR2                = 168;
static const int SPRITE_IDX_CURSOR3                = 169;
static const int SPRITE_IDX_CURSOR4                = 170;
static const int SPRITE_IDX_TITLE_FLAMES1          = 144;
static const int SPRITE_IDX_TITLE_FLAMES2          = 145;
static const int SPRITE_IDX_TITLE_FLAMES3          = 146;
static const int SPRITE_IDX_TITLE_FLAMES4          = 147;

static const int SPRITE_IDX_DAVE_RIGHT_HANDSFREE   = 53;
static const int SPRITE_IDX_DAVE_RIGHT_STAND       = 54;
static const int SPRITE_IDX_DAVE_RIGHT_SERIOUS     = 55;
static const int SPRITE_IDX_DAVE_FRONT             = 56;
static const int SPRITE_IDX_DAVE_LEFT_HANDSFREE    = 57;
static const int SPRITE_IDX_DAVE_LEFT_STAND        = 58;
static const int SPRITE_IDX_DAVE_LEFT_SERIOUS      = 59;
static const int SPRITE_IDX_DAVE_JUMP_RIGHT        = 67;
static const int SPRITE_IDX_DAVE_JUMP_LEFT         = 68;

static const int SPRITE_IDX_POPUP_BOX_T1           = 158;
static const int SPRITE_IDX_POPUP_BOX_T2           = 159;
static const int SPRITE_IDX_POPUP_BOX_T3           = 160;
static const int SPRITE_IDX_POPUP_BOX_M1           = 161;
static const int SPRITE_IDX_POPUP_BOX_M2           = 162;
static const int SPRITE_IDX_POPUP_BOX_M3           = 163;
static const int SPRITE_IDX_POPUP_BOX_B1           = 164;
static const int SPRITE_IDX_POPUP_BOX_B2           = 165;
static const int SPRITE_IDX_POPUP_BOX_B3           = 166;

static const int SPRITE_IDX_BOTTOM_BAR             = 171;
static const int SPRITES_MAX                       = 171;

#endif
