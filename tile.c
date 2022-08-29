#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tile.h"


/*
 * This should be called every tick (game minimal 'clock' beat) to let the tile do
 * their animation.
 */
void tile_tick(tile_t* tile) {
    tile->sprite_idx++;
    if (tile->sprites[tile->sprite_idx] == 0) {
        tile->sprite_idx = 0;
    }
}

/*
 * Returns the sprite idx to draw
 */
int tile_get_sprite(tile_t* tile) {
    return tile->sprites[tile->sprite_idx];
}

int tile_is_inside(tile_t* tile, int x, int y) {
    if (x >= tile->x && y >= tile->y &&
            x < (tile->x + tile->width) && y < (tile->y + tile->height)) {
        return 1;
    }
    return 0;
}

/*
 * Create a simple block tile (one sprite, doesn't animate)
 */
void tile_create_block(tile_t* t, int sprite, int x, int y, int width, int height) {
    t->x = x;
    t->y = y;
    t->width = width;
    t->height = height;

    t->sprites[0] = sprite;
    t->sprites[1] = 0;
    t->sprite_idx = 0;

    t->collision_dx = 0;
    t->collision_dy = 0;
    t->collision_dw = 0;
    t->collision_dh = 0;

    t->mod = BRICK;

    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
    t->is_inside = &tile_is_inside;
}

void tile_create_door(tile_t* t, int sprite, int x, int y, int width, int height) {
    t->x = x;
    t->y = y;
    t->width = width;
    t->height = height;
    t->sprites[0] = sprite;
    t->sprites[1] = 0;
    t->sprite_idx = 0;

    t->collision_dx = 0;
    t->collision_dy = 0;
    t->collision_dw = 0;
    t->collision_dh = 4;

    t->mod = DOOR;

    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
    t->is_inside = &tile_is_inside;
}

void tile_create_gun(tile_t* t, int sprite, int x, int y, int width, int height) {
    t->x = x;
    t->y = y;
    t->width = width;
    t->height = height;
    t->sprites[0] = sprite;
    t->sprites[1] = 0;
    t->sprite_idx = 0;

    t->collision_dx = 0;
    t->collision_dy = 0;
    t->collision_dw = 0;
    t->collision_dh = 0;

    t->mod = GUN;

    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
    t->is_inside = &tile_is_inside;
}

void tile_create_jetpack(tile_t* t, int sprite, int x, int y, int width, int height) {
    t->x = x;
    t->y = y;
    t->width = width;
    t->height = height;
    t->sprites[0] = sprite;
    t->sprites[1] = 0;
    t->sprite_idx = 0;

    t->collision_dx = 0;
    t->collision_dy = 0;
    t->collision_dw = 0;
    t->collision_dh = 0;

    t->mod = JETPACK;

    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
    t->is_inside = &tile_is_inside;
}

void tile_create_plasma_right(tile_t *t, int x, int y, int width, int height) {
    t->x = x;
    t->y = y;
    t->width = width;
    t->height = height;
    t->sprites[0] =  SPRITE_IDX_PLASMA_RIGHT1;
    t->sprites[1] =  SPRITE_IDX_PLASMA_RIGHT2;
    t->sprites[2] =  SPRITE_IDX_PLASMA_RIGHT3;
    t->sprites[3] = 0;
    t->sprite_idx = 0;

    t->collision_dx = 0;
    t->collision_dy = 0;
    t->collision_dw = 0;
    t->collision_dh = 0;

    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
    t->is_inside = &tile_is_inside;
}

void tile_create_plasma_left(tile_t *t, int x, int y, int width, int height) {
    t->x = x;
    t->y = y;
    t->width = width;
    t->height = height;
    t->sprites[0] =  SPRITE_IDX_PLASMA_LEFT1;
    t->sprites[1] =  SPRITE_IDX_PLASMA_LEFT2;
    t->sprites[2] =  SPRITE_IDX_PLASMA_LEFT3;
    t->sprites[3] = 0;
    t->sprite_idx = 0;

    t->collision_dx = 0;
    t->collision_dy = 0;
    t->collision_dw = 0;
    t->collision_dh = 0;

    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
    t->is_inside = &tile_is_inside;
}

/*
 * Create the flaming banner tile for into screen title
 */
void tile_create_intro_banner(tile_t* t, int x, int y) {
    t->x = x;
    t->y = y;
    t->width = 112;
    t->height = 47;
    t->sprites[0] = t->sprites[1] = t->sprites[2] = t->sprites[3] =
    t->sprites[4] = t->sprites[5] = SPRITE_IDX_TITLE_FLAMES1;

    t->sprites[6] = t->sprites[7] = t->sprites[8] = t->sprites[9] =
    t->sprites[10] = t->sprites[11] = SPRITE_IDX_TITLE_FLAMES2;

    t->sprites[12] = t->sprites[13] = t->sprites[14] = t->sprites[15] =
    t->sprites[16] = t->sprites[17] = SPRITE_IDX_TITLE_FLAMES3;

    t->sprites[18] = t->sprites[19] = t->sprites[20] = t->sprites[21] =
    t->sprites[22] = t->sprites[23] = SPRITE_IDX_TITLE_FLAMES4;

    t->sprites[24] = 0;
    t->sprite_idx = 0;

    t->collision_dx = 0;
    t->collision_dy = 0;
    t->collision_dw = 0;
    t->collision_dh = 0;

    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
    t->is_inside = &tile_is_inside;
}

/*
 * Create animated fire tile for intro. Its slightly different than in-game animated fire
 */
void tile_create_intro_fire(tile_t* t, int x, int y) {
    t->x = x;
    t->y = y;
    t->width = 16;
    t->height = 16;
    t->sprites[0] = t->sprites[1] = t->sprites[2] = t->sprites[3] =
    t->sprites[4] = t->sprites[5] = SPRITE_IDX_FIRE1;

    t->sprites[6] = t->sprites[7] = t->sprites[8] = t->sprites[9] =
    t->sprites[10] = t->sprites[11] = SPRITE_IDX_FIRE2;

    t->sprites[12] = t->sprites[13] = t->sprites[14] = t->sprites[15] =
    t->sprites[16] = t->sprites[17] = SPRITE_IDX_FIRE3;

    t->sprites[18] = t->sprites[19] = t->sprites[20] = t->sprites[21] =
    t->sprites[22] = t->sprites[23] = SPRITE_IDX_FIRE4;

    t->sprites[24] = 0;
    t->sprite_idx = 0;

    t->collision_dx = 0;
    t->collision_dy = 0;
    t->collision_dw = 0;
    t->collision_dh = 0;

    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
    t->is_inside = &tile_is_inside;
}

void tile_create_stars(tile_t *t, int x, int y) {
    t->x = x;
    t->y = y;
    t->width = 16;
    t->height = 16;
    t->sprites[0] = SPRITE_IDX_STARS;
    t->sprites[1] = 0;
    t->sprite_idx = 0;
    t->mod = CLIMB;

    t->collision_dx = 7;
    t->collision_dy = 7;
    t->collision_dw = -14;
    t->collision_dh = -4;

    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
    t->is_inside = &tile_is_inside;
}

void tile_create_stars_moon(tile_t *t, int x, int y) {
    t->x = x;
    t->y = y;
    t->width = 16;
    t->height = 16;
    t->sprites[0] = SPRITE_IDX_STARS_MOON;
    t->sprites[1] = 0;
    t->sprite_idx = 0;
    t->mod = CLIMB;

    t->collision_dx = 0;
    t->collision_dy = 3;
    t->collision_dw = 0;
    t->collision_dh = 0;

    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
    t->is_inside = &tile_is_inside;
}

void tile_create_moss(tile_t *t, int x, int y) {
    t->x = x;
    t->y = y;
    t->width = 16;
    t->height = 16;
    t->sprites[0] = SPRITE_IDX_MOSS;
    t->sprites[1] = 0;
    t->sprite_idx = 0;
    t->mod = MOSS;

    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
    t->is_inside = &tile_is_inside;
}

void tile_create_trunk(tile_t *t, int x, int y) {
    t->x = x;
    t->y = y;
    t->width = 16;
    t->height = 16;
    t->sprites[0] = SPRITE_IDX_TRUNK;
    t->sprites[1] = 0;
    t->sprite_idx = 0;
    t->mod = CLIMB;

    t->collision_dx = 0;
    t->collision_dy = 0;
    t->collision_dw = 0;
    t->collision_dh = 0;

    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
    t->is_inside = &tile_is_inside;
}

void tile_create_tree(tile_t* t, int x, int y, int type) {
    t->x = x;
    t->y = y;
    t->width = 16;
    t->height = 16;
    if (type == 0) {
        t->sprites[0] = SPRITE_IDX_TREE1;
    } else if (type == 1) {
        t->sprites[0] = SPRITE_IDX_TREE2;
    } else if (type == 2) {
        t->sprites[0] = SPRITE_IDX_TREE_CORNER_TOP_RIGHT;
    } else if (type == 3) {
        t->sprites[0] = SPRITE_IDX_TREE_CORNER_TOP_LEFT;
    } else if (type == 4) {
        t->sprites[0] = SPRITE_IDX_TREE_CORNER_BOT_RIGHT;
    } else if (type == 5) {
        t->sprites[0] = SPRITE_IDX_TREE_CORNER_BOT_LEFT;
    }

    t->sprites[1] = 0;
    t->sprite_idx = 0;
    t->mod = CLIMB;

    t->collision_dx = 0;
    t->collision_dy = 0;
    t->collision_dw = 0;
    t->collision_dh = 0;

    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
    t->is_inside = &tile_is_inside;
}

void tile_create_fire(tile_t* t, int x, int y, int idx_offset) {
    t->x = x;
    t->y = y;
    t->width = 16;
    t->height = 16;
    idx_offset = idx_offset * 10;

    t->sprites[0] = t->sprites[1] = t->sprites[2] = t->sprites[3] =
    t->sprites[4] = t->sprites[5] = t->sprites[6] = t->sprites[7] =
    t->sprites[8] = t->sprites[9] = SPRITE_IDX_FIRE1;

    t->sprites[10] = t->sprites[11] = t->sprites[12] = t->sprites[13] =
    t->sprites[14] = t->sprites[15] = t->sprites[16] = t->sprites[17] =
    t->sprites[18] = t->sprites[19] = SPRITE_IDX_FIRE2;

    t->sprites[20] = t->sprites[21] = t->sprites[22] = t->sprites[23] =
    t->sprites[24] = t->sprites[25] = t->sprites[26] = t->sprites[27] = 
    t->sprites[28] = t->sprites[29] = SPRITE_IDX_FIRE3;

    t->sprites[30] = t->sprites[31] = t->sprites[32] = t->sprites[33] =
    t->sprites[34] = t->sprites[35] = t->sprites[36] = t->sprites[37] = 
    t->sprites[38] = t->sprites[39] = SPRITE_IDX_FIRE4;

    t->sprites[40] = 0;
    t->sprite_idx = idx_offset;
    t->mod = FIRE;

    t->collision_dx = 6;
    t->collision_dy = 0;
    t->collision_dw = -12;
    t->collision_dh = 0;

    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
    t->is_inside = &tile_is_inside;
}

void tile_create_vines(tile_t* t, int x, int y, int idx_offset) {
    t->x = x;
    t->y = y;
    t->width = 16;
    t->height = 16;
    idx_offset = idx_offset * 10;

    t->sprites[0] = t->sprites[1] = t->sprites[2] = t->sprites[3] =
    t->sprites[4] = t->sprites[5] = t->sprites[6] = t->sprites[7] =
    t->sprites[8] = t->sprites[9] = SPRITE_IDX_VINES1;

    t->sprites[10] = t->sprites[11] = t->sprites[12] = t->sprites[13] =
    t->sprites[14] = t->sprites[15] = t->sprites[16] = t->sprites[17] =
    t->sprites[18] = t->sprites[19] = SPRITE_IDX_VINES2;

    t->sprites[20] = t->sprites[21] = t->sprites[22] = t->sprites[23] =
    t->sprites[24] = t->sprites[25] = t->sprites[26] = t->sprites[27] = 
    t->sprites[28] = t->sprites[29] = SPRITE_IDX_VINES3;

    t->sprites[30] = t->sprites[31] = t->sprites[32] = t->sprites[33] =
    t->sprites[34] = t->sprites[35] = t->sprites[36] = t->sprites[37] = 
    t->sprites[38] = t->sprites[39] = SPRITE_IDX_VINES4;

    t->sprites[40] = 0;
    t->sprite_idx = idx_offset;

    t->mod = FIRE;
    t->collision_dx = 6;
    t->collision_dy = 0;
    t->collision_dw = -12;
    t->collision_dh = 0;

    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
    t->is_inside = &tile_is_inside;
}

void tile_create_water(tile_t* t, int x, int y, int idx_offset) {
    t->x = x;
    t->y = y;
    t->width = 16;
    t->height = 16;
    idx_offset = idx_offset * 10;
    t->sprites[0] = t->sprites[1] = t->sprites[2] = t->sprites[3] =
    t->sprites[4] = t->sprites[5] = t->sprites[6] = t->sprites[7] =
    t->sprites[8] = t->sprites[9] = SPRITE_IDX_WATER1;

    t->sprites[10] = t->sprites[11] = t->sprites[12] = t->sprites[13] =
    t->sprites[14] = t->sprites[15] = t->sprites[16] = t->sprites[17] =
    t->sprites[18] = t->sprites[19] = SPRITE_IDX_WATER2;

    t->sprites[20] = t->sprites[21] = t->sprites[22] = t->sprites[23] =
    t->sprites[24] = t->sprites[25] = t->sprites[26] = t->sprites[27] =
    t->sprites[28] = t->sprites[29] = SPRITE_IDX_WATER3;

    t->sprites[30] = t->sprites[31] = t->sprites[32] = t->sprites[33] =
    t->sprites[34] = t->sprites[35] = t->sprites[36] = t->sprites[37] =
    t->sprites[38] = t->sprites[39] = SPRITE_IDX_WATER4;

    t->sprites[40] = t->sprites[41] = t->sprites[42] = t->sprites[43] =
    t->sprites[44] = t->sprites[45] = t->sprites[46] = t->sprites[47] =
    t->sprites[48] = t->sprites[49] = SPRITE_IDX_WATER5;

    t->sprites[50] = 0;
    t->sprite_idx = idx_offset;
    t->mod = FIRE;

    t->collision_dx = 6;
    t->collision_dy = 4;
    t->collision_dw = -12;
    t->collision_dh = -8;

    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
    t->is_inside = &tile_is_inside;
}

void tile_create_flashing_cursor(tile_t* t, int x, int y) {
    t->x = x;
    t->y = y;
    t->width = 8;
    t->height = 8;
    t->sprites[0] = t->sprites[1] = t->sprites[2] = t->sprites[3] =
    t->sprites[4] = SPRITE_IDX_CURSOR1;

    t->sprites[5] = t->sprites[6] = t->sprites[7] = t->sprites[8] =
    t->sprites[9] = SPRITE_IDX_CURSOR2;

    t->sprites[10] = t->sprites[11] = t->sprites[12] = t->sprites[13] =
    t->sprites[14] = SPRITE_IDX_CURSOR3;

    t->sprites[15] = t->sprites[16] = t->sprites[17] = t->sprites[18] =
    t->sprites[19] = SPRITE_IDX_CURSOR4;

    t->sprites[20] = 0;
    t->sprite_idx = 0;

    t->collision_dx = 0;
    t->collision_dy = 0;
    t->collision_dw = 0;
    t->collision_dh = 0;

    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
}

void tile_create_top_separator(tile_t* t, int x, int y) {
    t->x = x;
    t->y = y;
    t->width = 320;
    t->height = 4;
    t->sprites[0] = SPRITE_IDX_TOP_BAR;
    t->sprites[1] = 0;
    t->sprite_idx = 0;
    t->collision_dx = 0;
    t->collision_dy = 0;
    t->collision_dw = 0;
    t->collision_dh = 0;
    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
}

void tile_create_bottom_separator(tile_t* t, int x, int y) {
    t->x = x;
    t->y = y;
    t->width = 320;
    t->height = 16;
    t->sprites[0] = SPRITE_IDX_BOTTOM_BAR;
    t->sprites[1] = 0;
    t->sprite_idx = 0;
    t->collision_dx = 0;
    t->collision_dy = 0;
    t->collision_dw = 0;
    t->collision_dh = 0;
    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
}

void tile_create_grail_banner(tile_t* t, int x, int y) {
    t->x = x;
    t->y = y;
    t->width = 176;
    t->height = 14;
    t->sprites[0] = SPRITE_IDX_TROPHY_BANNER;
    t->sprites[1] = 0;
    t->sprite_idx = 0;
    t->collision_dx = 0;
    t->collision_dy = 0;
    t->collision_dw = 0;
    t->collision_dh = 0;
    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
}

void tile_create_label_warp(tile_t *t, int x, int y) {
    t->x = x;
    t->y = y;
    t->width = 72;
    t->height = 15;
    t->sprites[0] = SPRITE_IDX_LABEL_WARP;
    t->sprites[1] = 0;
    t->sprite_idx = 0;
    t->collision_dx = 0;
    t->collision_dy = 0;
    t->collision_dw = 0;
    t->collision_dh = 0;
    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
}

void tile_create_label_zone(tile_t *t, int x, int y) {
    t->x = x;
    t->y = y;
    t->width = 68;
    t->height = 15;
    t->sprites[0] = SPRITE_IDX_LABEL_ZONE;
    t->sprites[1] = 0;
    t->sprite_idx = 0;
    t->collision_dx = 0;
    t->collision_dy = 0;
    t->collision_dw = 0;
    t->collision_dh = 0;
    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
}

void tile_create_gun_banner(tile_t *t, int x, int y) {
    t->x = x;
    t->y = y;
    t->width = 62;
    t->height = 11;
    t->sprites[0] = SPRITE_IDX_GUN_BANNER;
    t->sprites[1] = 0;
    t->sprite_idx = 0;
    t->collision_dx = 0;
    t->collision_dy = 0;
    t->collision_dw = 0;
    t->collision_dh = 0;
    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
}

void tile_create_purple_gem(tile_t *t, int x, int y) {
    t->x = x;
    t->y = y;
    t->width = 16;
    t->height = 16;
    t->sprites[0] = SPRITE_IDX_PURPLE_GEM;
    t->sprites[1] = 0;
    t->sprite_idx = 0;
    t->collision_dx = 0;
    t->collision_dy = 0;
    t->collision_dw = 0;
    t->collision_dh = 0;
    t->mod = LOOT;
    t->score_value = 50;

    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
    t->is_inside = &tile_is_inside;
}

void tile_create_teal_gem(tile_t *t, int x, int y) {
    t->x = x;
    t->y = y;
    t->width = 16;
    t->height = 16;
    t->sprites[0] = SPRITE_IDX_TEAL_GEM;
    t->sprites[1] = 0;
    t->sprite_idx = 0;

    t->collision_dx = 0;
    t->collision_dy = 0;
    t->collision_dw = 1;
    t->collision_dh = 0;

    t->mod = LOOT;
    t->score_value = 100;

    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
    t->is_inside = &tile_is_inside;
}

void tile_create_red_gem(tile_t *t, int x, int y) {
    t->x = x;
    t->y = y;
    t->width = 16;
    t->height = 16;
    t->sprites[0] = SPRITE_IDX_RED_GEM;
    t->sprites[1] = 0;
    t->sprite_idx = 0;
    t->collision_dx = 0;
    t->collision_dy = 0;
    t->collision_dw = 0;
    t->collision_dh = 0;
    t->mod = LOOT;
    t->score_value = 150;

    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
    t->is_inside = &tile_is_inside;
}

void tile_create_ring(tile_t *t, int x, int y) {
    t->x = x;
    t->y = y;
    t->width = 16;
    t->height = 16;
    t->sprites[0] = SPRITE_IDX_RING;
    t->sprites[1] = 0;
    t->sprite_idx = 0;
    t->collision_dx = 0;
    t->collision_dy = 0;
    t->collision_dw = 0;
    t->collision_dh = 0;
    t->mod = LOOT;
    t->score_value = 550;

    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
    t->is_inside = &tile_is_inside;
}

void tile_create_crown(tile_t *t, int x, int y) {
    t->x = x;
    t->y = y;
    t->width = 16;
    t->height = 16;
    t->sprites[0] = SPRITE_IDX_CROWN;
    t->sprites[1] = 0;
    t->sprite_idx = 0;
    t->collision_dx = 0;
    t->collision_dy = 0;
    t->collision_dw = 0;
    t->collision_dh = 0;
    t->mod = LOOT;
    t->score_value = 800;

    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
    t->is_inside = &tile_is_inside;
}

void tile_create_scepter(tile_t *t, int x, int y) {
    t->x = x;
    t->y = y;
    t->width = 16;
    t->height = 16;
    t->sprites[0] = SPRITE_IDX_SCEPTER;
    t->sprites[1] = 0;
    t->sprite_idx = 0;
    t->collision_dx = 0;
    t->collision_dy = 0;
    t->collision_dw = 0;
    t->collision_dh = 0;
    t->mod = LOOT;
    t->score_value = 400;

    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
    t->is_inside = &tile_is_inside;
}

void tile_create_grail(tile_t *t, int x, int y) {
    t->x = x;
    t->y = y;
    t->width = 16;
    t->height = 16;

    t->sprites[0] = t->sprites[1] = t->sprites[2] = t->sprites[3] =
    t->sprites[4] = t->sprites[5] = t->sprites[6] = t->sprites[7] =
    t->sprites[8] = t->sprites[9] = SPRITE_IDX_TROPHY0;

    t->sprites[10] = t->sprites[11] = t->sprites[12] = t->sprites[13] =
    t->sprites[14] = t->sprites[15] = t->sprites[16] = t->sprites[17] =
    t->sprites[18] = t->sprites[19] = SPRITE_IDX_TROPHY1;

    t->sprites[20] = t->sprites[21] = t->sprites[22] = t->sprites[23] =
    t->sprites[24] = t->sprites[25] = t->sprites[26] = t->sprites[27] =
    t->sprites[28] = t->sprites[29] = SPRITE_IDX_TROPHY2;

    t->sprites[30] = t->sprites[31] = t->sprites[32] = t->sprites[33] =
    t->sprites[34] = t->sprites[35] = t->sprites[36] = t->sprites[37] =
    t->sprites[38] = t->sprites[39] = SPRITE_IDX_TROPHY3;

    t->sprites[40] = t->sprites[41] = t->sprites[42] = t->sprites[43] =
    t->sprites[44] = t->sprites[45] = t->sprites[46] = t->sprites[47] =
    t->sprites[48] = t->sprites[49] = SPRITE_IDX_TROPHY4;

    t->sprites[50] = 0;
    t->sprite_idx = 0;
    t->collision_dx = 0;
    t->collision_dy = 0;
    t->collision_dw = 0;
    t->collision_dh = 0;
    t->mod = TROPHY;
    t->score_value = 1000;

    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
    t->is_inside = &tile_is_inside;
}

void tile_create(tile_t* t, char tag[4], int x, int y) {
    if (strcmp(tag, "RBK") == 0) {
        tile_create_block(t, SPRITE_IDX_RED_BRICK, x, y, 16, 16);
    } else if (strcmp(tag, "BBK") == 0) {
        tile_create_block(t, SPRITE_IDX_BLUE_BRICK, x, y, 16, 16);
    } else if (strcmp(tag, "PPK") == 0) {
        tile_create_block(t, SPRITE_IDX_PURPLE_PLATFORM, x, y, 16, 16);
    } else if (strcmp(tag, "PCK") == 0) {
        tile_create_block(t, SPRITE_IDX_PURPLE_COLUMN, x, y, 16, 16);
    } else if (strcmp(tag, "DRT") == 0) {
        tile_create_block(t, SPRITE_IDX_DIRT, x, y, 16, 16);
    } else if (strcmp(tag, "BIM") == 0) {
        tile_create_block(t, SPRITE_IDX_METAL_BEAM, x, y, 16, 16);
    } else if (strcmp(tag, "DRB") == 0) {
        tile_create_block(t, SPRITE_IDX_DIRT_BLOOD, x, y, 16, 16);
    } else if (strcmp(tag, "BCM") == 0) {
        tile_create_block(t, SPRITE_IDX_BLUE_COLUMN, x, y, 16, 16);
    } else if (strcmp(tag, "PIR") == 0) {
        tile_create_block(t, SPRITE_IDX_PIPE_RIGHT, x, y, 16, 16);
    } else if (strcmp(tag, " X ") == 0) {
        tile_create_door(t, SPRITE_IDX_DOOR, x, y, 16, 16);
    } else if (strcmp(tag, "GUN") == 0) {
        tile_create_gun(t, SPRITE_IDX_GUN, x, y, 16, 16);
    } else if (strcmp(tag, "JPK") == 0) {
        tile_create_jetpack(t, SPRITE_IDX_JETPACK, x, y, 16, 16);
    } else if (strcmp(tag, " * ") == 0) {
        tile_create_purple_gem(t, x, y);
    } else if (strcmp(tag, " v ") == 0) {
        tile_create_teal_gem(t, x, y);
    } else if (strcmp(tag, " V ") == 0) {
        tile_create_red_gem(t, x, y);
    } else if (strcmp(tag, " O ") == 0) {
        tile_create_ring(t, x, y);
    } else if (strcmp(tag, " W ") == 0) {
        tile_create_crown(t, x, y);
    } else if (strcmp(tag, " ! ") == 0) {
        tile_create_scepter(t, x, y);
    } else if (strcmp(tag, " Y ") == 0) {
        tile_create_grail(t, x, y);
    } else if (strcmp(tag, "FR1") == 0) {
        tile_create_fire(t, x, y, 0);
    } else if (strcmp(tag, "FR2") == 0) {
        tile_create_fire(t, x, y, 1);
    } else if (strcmp(tag, "FR3") == 0) {
        tile_create_fire(t, x, y, 2);
    } else if (strcmp(tag, "FR4") == 0) {
        tile_create_fire(t, x, y, 3);
    } else if (strcmp(tag, "WT1") == 0) {
        tile_create_water(t, x, y, 0);
    } else if (strcmp(tag, "WT2") == 0) {
        tile_create_water(t, x, y, 1);
    } else if (strcmp(tag, "WT3") == 0) {
        tile_create_water(t, x, y, 2);
    } else if (strcmp(tag, "WT4") == 0) {
        tile_create_water(t, x, y, 3);
    } else if (strcmp(tag, "WT5") == 0) {
        tile_create_water(t, x, y, 4);
    } else if (strcmp(tag, "VI1") == 0) {
        tile_create_vines(t, x, y, 0);
    } else if (strcmp(tag, "VI2") == 0) {
        tile_create_vines(t, x, y, 1);
    } else if (strcmp(tag, "VI3") == 0) {
        tile_create_vines(t, x, y, 2);
    } else if (strcmp(tag, "VI4") == 0) {
        tile_create_vines(t, x, y, 3);
    } else if (strcmp(tag, "DR1") == 0) {
        tile_create_block(t, SPRITE_IDX_DIRT_TOP_RIGHT, x, y, 16, 16);
    } else if (strcmp(tag, "DR2") == 0) {
        tile_create_block(t, SPRITE_IDX_DIRT_BOTTOM_RIGHT, x, y, 16, 16);
    } else if (strcmp(tag, "DR3") == 0) {
        tile_create_block(t, SPRITE_IDX_DIRT_BOTTOM_LEFT, x, y, 16, 16);
    } else if (strcmp(tag, "DR4") == 0) {
        tile_create_block(t, SPRITE_IDX_DIRT_TOP_LEFT, x, y, 16, 16);
    } else if ((strcmp(tag, "  M") == 0) || (strcmp(tag, "D+M") == 0)) {
        tile_create_moss(t, x, y);
    } else if (strcmp(tag, "TRK") == 0) {
        tile_create_trunk(t, x, y);
    } else if (strcmp(tag, "TR1") == 0) {
        tile_create_tree(t, x, y, 0);
    } else if (strcmp(tag, "TR2") == 0) {
        tile_create_tree(t, x, y, 1);
    } else if (strcmp(tag, "TR3") == 0) {
        tile_create_tree(t, x, y, 2);
    } else if (strcmp(tag, "TR4") == 0) {
        tile_create_tree(t, x, y, 3);
    } else if (strcmp(tag, "TR5") == 0) {
        tile_create_tree(t, x, y, 4);
    } else if (strcmp(tag, "TR6") == 0) {
        tile_create_tree(t, x, y, 5);
    } else if (strcmp(tag, "STR") == 0) {
        tile_create_stars(t, x, y);
    } else if (strcmp(tag, "MON") == 0) {
        tile_create_stars_moon(t, x, y);
    }
}

