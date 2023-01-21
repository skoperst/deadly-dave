#ifndef _TILE_H_
#define _TILE_H_

#include <stdio.h>
#include <stdint.h>

/*
 * Quick conversion between grid and pixel basis
 */
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

/*
 * Modifiers of tiles, sort of logical category/characteristic.
 * So for example dirt block and red brick block will be 'brick', or
 * vines and fire both will be fire as they cause dave to get burned by it
 */
static const int EMPTY      = 0;
static const int CLEAR      = 1;
static const int BRICK      = 2;
static const int LOOT       = 3;
static const int TROPHY     = 4;
static const int MOSS       = 5;
static const int CLIMB      = 6;
static const int FIRE       = 7;
static const int DOOR       = 8;
static const int DAVE       = 9;
static const int GUN        = 10;
static const int JETPACK    = 11;
static const int MONSTER    = 12;


/*
 * Each index represent a sprite loaded from resource images
 */
static const int SPRITE_IDX_DIRT_BLOOD             = 1;
static const int SPRITE_IDX_DOOR                   = 2;
static const int SPRITE_IDX_METAL_BEAM             = 3;
static const int SPRITE_IDX_JETPACK                = 4;
static const int SPRITE_IDX_BLUE_BRICK             = 5;
static const int SPRITE_IDX_FIRE1                  = 6;
static const int SPRITE_IDX_FIRE2                  = 7;
static const int SPRITE_IDX_FIRE3                  = 8;
static const int SPRITE_IDX_FIRE4                  = 9;
static const int SPRITE_IDX_TROPHY2                = 10;
static const int SPRITE_IDX_TROPHY3                = 11;
static const int SPRITE_IDX_TROPHY4                = 12;
static const int SPRITE_IDX_TROPHY0                = 13;
static const int SPRITE_IDX_TROPHY1                = 14;
static const int SPRITE_IDX_PIPE_RIGHT             = 15;
static const int SPRITE_IDX_PIPE_DOWN              = 16;
static const int SPRITE_IDX_RED_BRICK              = 17;
static const int SPRITE_IDX_DIRT                   = 18;
static const int SPRITE_IDX_BLUE_COLUMN            = 19;
static const int SPRITE_IDX_GUN                    = 20;
static const int SPRITE_IDX_DIRT_BOTTOM_LEFT       = 21;
static const int SPRITE_IDX_DIRT_TOP_LEFT          = 22;
static const int SPRITE_IDX_DIRT_TOP_RIGHT         = 23;
static const int SPRITE_IDX_DIRT_BOTTOM_RIGHT      = 24;
static const int SPRITE_IDX_VINES1                 = 25;
static const int SPRITE_IDX_VINES2                 = 26;
static const int SPRITE_IDX_VINES3                 = 27;
static const int SPRITE_IDX_VINES4                 = 28;
static const int SPRITE_IDX_PURPLE_COLUMN          = 29;
static const int SPRITE_IDX_PURPLE_PLATFORM        = 30;
static const int SPRITE_IDX_MOSS                   = 32;
static const int SPRITE_IDX_TRUNK                  = 33;
static const int SPRITE_IDX_TREE1                  = 34;
static const int SPRITE_IDX_TREE2                  = 35;
static const int SPRITE_IDX_WATER1                 = 36;
static const int SPRITE_IDX_WATER2                 = 37;
static const int SPRITE_IDX_WATER3                 = 38;
static const int SPRITE_IDX_WATER4                 = 39;
static const int SPRITE_IDX_WATER5                 = 40;
static const int SPRITE_IDX_STARS                  = 41;
static const int SPRITE_IDX_STARS_MOON             = 42;
static const int SPRITE_IDX_TREE_CORNER_TOP_LEFT   = 43;
static const int SPRITE_IDX_TREE_CORNER_TOP_RIGHT  = 44;
static const int SPRITE_IDX_TREE_CORNER_BOT_LEFT   = 45;
static const int SPRITE_IDX_TREE_CORNER_BOT_RIGHT  = 46;
static const int SPRITE_IDX_TEAL_GEM               = 47;
static const int SPRITE_IDX_PURPLE_GEM             = 48;
static const int SPRITE_IDX_RED_GEM                = 49;
static const int SPRITE_IDX_CROWN                  = 50;
static const int SPRITE_IDX_RING                   = 51;
static const int SPRITE_IDX_SCEPTER                = 52;
static const int SPRITE_IDX_DAVE_RIGHT_HANDSFREE   = 53;
static const int SPRITE_IDX_DAVE_RIGHT_STAND       = 54;
static const int SPRITE_IDX_DAVE_RIGHT_SERIOUS     = 55;
static const int SPRITE_IDX_DAVE_FRONT             = 56;
static const int SPRITE_IDX_DAVE_LEFT_HANDSFREE    = 57;
static const int SPRITE_IDX_DAVE_LEFT_STAND        = 58;
static const int SPRITE_IDX_DAVE_LEFT_SERIOUS      = 59;
static const int SPRITE_IDX_DAVE_JUMP_RIGHT        = 67;
static const int SPRITE_IDX_DAVE_JUMP_LEFT         = 68;
static const int SPRITE_IDX_DAVE_CLIMB_HANDS_UP    = 71;
static const int SPRITE_IDX_DAVE_CLIMB_HAND_RIGHT  = 72;
static const int SPRITE_IDX_DAVE_CLIMB_HAND_LEFT   = 73;
static const int SPRITE_IDX_DAVE_JETPACK_RIGHT1    = 77;
static const int SPRITE_IDX_DAVE_JETPACK_RIGHT2    = 78;
static const int SPRITE_IDX_DAVE_JETPACK_RIGHT3    = 79;
static const int SPRITE_IDX_DAVE_JETPACK_LEFT1     = 80;
static const int SPRITE_IDX_DAVE_JETPACK_LEFT2     = 81;
static const int SPRITE_IDX_DAVE_JETPACK_LEFT3     = 82;
static const int SPRITE_IDX_MONSTER_SPIDER1        = 89;
static const int SPRITE_IDX_MONSTER_SPIDER2        = 90;
static const int SPRITE_IDX_MONSTER_SPIDER3        = 91;
static const int SPRITE_IDX_MONSTER_SPIDER4        = 92;
static const int SPRITE_IDX_MONSTER_SWIRL1         = 93;
static const int SPRITE_IDX_MONSTER_SWIRL2         = 94;
static const int SPRITE_IDX_MONSTER_SWIRL3         = 95;
static const int SPRITE_IDX_MONSTER_SWIRL4         = 96;
static const int SPRITE_IDX_MONSTER_SUN1           = 97;
static const int SPRITE_IDX_MONSTER_SUN2           = 98;
static const int SPRITE_IDX_MONSTER_SUN3           = 99;
static const int SPRITE_IDX_MONSTER_SUN4           = 100;
static const int SPRITE_IDX_MONSTER_BONES1         = 101;
static const int SPRITE_IDX_MONSTER_BONES2         = 102;
static const int SPRITE_IDX_MONSTER_BONES3         = 103;
static const int SPRITE_IDX_MONSTER_BONES4         = 104;
static const int SPRITE_IDX_MONSTER_UFO1           = 105;
static const int SPRITE_IDX_MONSTER_UFO2           = 106;
static const int SPRITE_IDX_MONSTER_UFO3           = 107;
static const int SPRITE_IDX_MONSTER_UFO4           = 108;
static const int SPRITE_IDX_MONSTER_GUARD1         = 109;
static const int SPRITE_IDX_MONSTER_GUARD2         = 110;
static const int SPRITE_IDX_MONSTER_GUARD3         = 111;
static const int SPRITE_IDX_MONSTER_GUARD4         = 112;
static const int SPRITE_IDX_MONSTER_ENVY1          = 113;
static const int SPRITE_IDX_MONSTER_ENVY2          = 114;
static const int SPRITE_IDX_MONSTER_ENVY3          = 115;
static const int SPRITE_IDX_MONSTER_ENVY4          = 116;
static const int SPRITE_IDX_MONSTER_PLATE1         = 117;
static const int SPRITE_IDX_MONSTER_PLATE2         = 118;
static const int SPRITE_IDX_MONSTER_PLATE3         = 119;
static const int SPRITE_IDX_MONSTER_PLATE4         = 120;
static const int SPRITE_IDX_PLASMA_RIGHT1          = 121;
static const int SPRITE_IDX_PLASMA_RIGHT2          = 122;
static const int SPRITE_IDX_PLASMA_RIGHT3          = 123;
static const int SPRITE_IDX_PLASMA_LEFT1           = 124;
static const int SPRITE_IDX_PLASMA_LEFT2           = 125;
static const int SPRITE_IDX_PLASMA_LEFT3           = 126;
static const int SPRITE_IDX_BULLET_RIGHT           = 127;
static const int SPRITE_IDX_BULLET_LEFT            = 128;
static const int SPRITE_IDX_EXPLOSION1             = 129;
static const int SPRITE_IDX_EXPLOSION2             = 130;
static const int SPRITE_IDX_EXPLOSION3             = 131;
static const int SPRITE_IDX_EXPLOSION4             = 132;
static const int SPRITE_IDX_JETPACK_LABEL          = 133;
static const int SPRITE_IDX_GUN_BANNER             = 134;
static const int SPRITE_IDX_TROPHY_BANNER          = 138;
static const int SPRITE_IDX_LABEL_WARP             = 139;
static const int SPRITE_IDX_LABEL_ZONE             = 140;
static const int SPRITE_IDX_JETPACK_BAR_FRAME      = 141;
static const int SPRITE_IDX_JETPACK_BAR            = 142;
static const int SPRITE_IDX_TITLE_FLAMES1          = 144;
static const int SPRITE_IDX_TITLE_FLAMES2          = 145;
static const int SPRITE_IDX_TITLE_FLAMES3          = 146;
static const int SPRITE_IDX_TITLE_FLAMES4          = 147;
static const int SPRITE_IDX_POPUP_BOX_T1           = 158;
static const int SPRITE_IDX_POPUP_BOX_T2           = 159;
static const int SPRITE_IDX_POPUP_BOX_T3           = 160;
static const int SPRITE_IDX_POPUP_BOX_M1           = 161;
static const int SPRITE_IDX_POPUP_BOX_M2           = 162;
static const int SPRITE_IDX_POPUP_BOX_M3           = 163;
static const int SPRITE_IDX_POPUP_BOX_B1           = 164;
static const int SPRITE_IDX_POPUP_BOX_B2           = 165;
static const int SPRITE_IDX_POPUP_BOX_B3           = 166;
static const int SPRITE_IDX_CURSOR1                = 167;
static const int SPRITE_IDX_CURSOR2                = 168;
static const int SPRITE_IDX_CURSOR3                = 169;
static const int SPRITE_IDX_CURSOR4                = 170;
static const int SPRITE_IDX_BOTTOM_BAR             = 171;
static const int SPRITE_IDX_TOP_BAR                = 172;
static const int SPRITES_MAX                       = 172;


/*
 * The tile is the basic building of tile-based graphics. Each tile will hold the information
 * of one game piece which can be interactive and can be idle, can animate or be idle,
 * can move and change its parameters.
 */
typedef struct tile_struct {
    /* Pointer to the owner of this tile */
    void *context;

    int x;
    int y;

    int width;
    int height;

    /*
     * The delta (difference) in x,y,w,h for the collision box, so for example when
     * all 0, it means collision box is exactly rect x,y,w,h. If we want the collision box 
     * to be bigger in 1 pixel, we will set dx = -1, dy = -1, dw = 2, dh = 2. Note the change in width
     * and height is 2 as we need eventually to add one pixel at each side 
     */
    int collision_dx;
    int collision_dy;
    int collision_dw;
    int collision_dh;

    int mod;
    int sprites[100];
    int sprite_idx;
    int score_value;

    /* A tick is a function owner of a tile calls so the tile can 'do its thing', whatever it might be. */
    void (*tick)(struct tile_struct *tile);

    /* Returns the sprite index to draw for this tile, 0 will indicate no need to draw anything */
    int (*get_sprite)(struct tile_struct *tile);

    /* Returns 1 if x,y is inside the tile's given rect: x,y,w,h */
    int (*is_inside)(struct tile_struct *tile, int x, int y);
} tile_t;

/* Create simple non-interacting block */
void tile_create_block(tile_t* t, int sprite, int x, int y, int width, int height);

/* Intro related */
void tile_create_intro_fire(tile_t* t, int x, int y);
void tile_create_intro_banner(tile_t* t, int x, int y);

void tile_create_plasma_right(tile_t *t, int x, int y, int width, int height);
void tile_create_plasma_left(tile_t *t, int x, int y, int width, int height);
/*
 * Tiles for special purpose
 */
void tile_create_grail_banner(tile_t *t, int x, int y);
void tile_create_gun_banner(tile_t *t, int x, int y);
void tile_create_label_warp(tile_t *t, int x, int y);
void tile_create_label_zone(tile_t *t, int x, int y);

/*
 * Popup box
 */
void tile_create_flashing_cursor(tile_t* t, int x, int y);

/* Game screen top & bottom bars */
void tile_create_bottom_separator(tile_t* t, int x, int y);
void tile_create_top_separator(tile_t* t, int x, int y);

void tile_create(tile_t* t, char tag[4], int x, int y);

int tile_file_parse(tile_t* map, int *x, int *y, const char* path);
int tile_map_parse(tile_t* map, int *x, int *y, char* map_str);

#endif
