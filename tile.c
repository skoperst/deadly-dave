#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tile.h"


// This called every game 'clock' beat (tick).
// In case of tiles, they just change their sprite from time to time.
void tile_tick(tile_t* tile)
{
    tile->sprite_idx++;
    if (tile->sprites[tile->sprite_idx] == 0) {
        tile->sprite_idx = 0;
    }
}

// Returns the sprite idx to draw
int tile_get_sprite(tile_t* tile)
{
    return tile->sprites[tile->sprite_idx];
}

int tile_is_inside(tile_t* tile, int x, int y)
{
    if (x >= tile->x && y >= tile->y && 
            x < (tile->x + tile->width) && y < (tile->y + tile->height)) {
        return 1;
    }

    return 0;
}

/*
 * Create a simple block tile (one sprite, doesn't animate)
 */
void tile_create_block(tile_t* t, int sprite, int x, int y, int width, int height)
{
    t->x = x;
    t->y = y;
    t->width = width;
    t->height = height;
    t->sprites[0] = sprite;
    t->sprites[1] = 0;
    t->sprite_idx = 0;
    t->mod = BRICK;

    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
    t->is_inside = &tile_is_inside;
}

/*
 * Create the flaming banner tile for into screen title
 */
void tile_create_intro_banner(tile_t* t, int x, int y)
{
    t->x = x;
    t->y = y;
    t->width = 112;
    t->height = 47;
    t->sprites[0] = SPRITE_IDX_TITLE_FLAMES1;
    t->sprites[1] = SPRITE_IDX_TITLE_FLAMES1;
    t->sprites[2] = SPRITE_IDX_TITLE_FLAMES1;
    t->sprites[3] = SPRITE_IDX_TITLE_FLAMES1;
    t->sprites[4] = SPRITE_IDX_TITLE_FLAMES1;
    t->sprites[5] = SPRITE_IDX_TITLE_FLAMES1;

    t->sprites[6] = SPRITE_IDX_TITLE_FLAMES2;
    t->sprites[7] = SPRITE_IDX_TITLE_FLAMES2;
    t->sprites[8] = SPRITE_IDX_TITLE_FLAMES2;
    t->sprites[9] = SPRITE_IDX_TITLE_FLAMES2;
    t->sprites[10] = SPRITE_IDX_TITLE_FLAMES2;
    t->sprites[11] = SPRITE_IDX_TITLE_FLAMES2;

    t->sprites[12] = SPRITE_IDX_TITLE_FLAMES3;
    t->sprites[13] = SPRITE_IDX_TITLE_FLAMES3;
    t->sprites[14] = SPRITE_IDX_TITLE_FLAMES3;
    t->sprites[15] = SPRITE_IDX_TITLE_FLAMES3;
    t->sprites[16] = SPRITE_IDX_TITLE_FLAMES3;
    t->sprites[17] = SPRITE_IDX_TITLE_FLAMES3;

    t->sprites[18] = SPRITE_IDX_TITLE_FLAMES4;
    t->sprites[19] = SPRITE_IDX_TITLE_FLAMES4;
    t->sprites[20] = SPRITE_IDX_TITLE_FLAMES4;
    t->sprites[21] = SPRITE_IDX_TITLE_FLAMES4;
    t->sprites[22] = SPRITE_IDX_TITLE_FLAMES4;
    t->sprites[23] = SPRITE_IDX_TITLE_FLAMES4;

    t->sprites[24] = 0;
    t->sprite_idx = 0;
    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
    t->is_inside = &tile_is_inside;
}

void tile_create_intro_fire(tile_t* t, int x, int y)
{
    t->x = x;
    t->y = y;
    t->width = 16;
    t->height = 16;
    t->sprites[0] = t->sprites[1] = t->sprites[2] = t->sprites[3] = t->sprites[4] = t->sprites[5] = SPRITE_IDX_FIRE1;
    t->sprites[6] = t->sprites[7] = t->sprites[8] = t->sprites[9] = t->sprites[10] = t->sprites[11] = SPRITE_IDX_FIRE2;
    t->sprites[12] = t->sprites[13] = t->sprites[14] = t->sprites[15] = t->sprites[16] = t->sprites[17] = SPRITE_IDX_FIRE3;
    t->sprites[18] = t->sprites[19] = t->sprites[20] = t->sprites[21] = t->sprites[22] = t->sprites[23] = SPRITE_IDX_FIRE4;

    t->sprites[24] = 0;
    t->sprite_idx = 0;
    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
    t->is_inside = &tile_is_inside;
}

void tile_create_flashing_cursor(tile_t* t, int x, int y)
{
        t->x = x;
        t->y = y;
        t->width = 8;
        t->height = 8;
        t->sprites[0] = t->sprites[1] = t->sprites[2] = t->sprites[3] = t->sprites[4] = SPRITE_IDX_CURSOR1;
        t->sprites[5] = t->sprites[6] = t->sprites[7] = t->sprites[8] = t->sprites[9] = SPRITE_IDX_CURSOR2;
        t->sprites[10] = t->sprites[11] = t->sprites[12] = t->sprites[13] = t->sprites[14] = SPRITE_IDX_CURSOR3;
        t->sprites[15] = t->sprites[16] = t->sprites[17] = t->sprites[18] = t->sprites[19] = SPRITE_IDX_CURSOR4;
        t->sprites[20] = 0;
        t->sprite_idx = 0;
        t->get_sprite = &tile_get_sprite;
        t->tick = &tile_tick;
}

void tile_create_top_separator(tile_t* t, int x, int y)
{
    t->x = x;
    t->y = y;
    t->width = 320;
    t->height = 4;
    t->sprites[0] = SPRITE_IDX_TOP_BAR;
    t->sprites[1] = 0;
    t->sprite_idx = 0;
    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
}

void tile_create_bottom_separator(tile_t* t, int x, int y)
{
    t->x = x;
    t->y = y;
    t->width = 320;
    t->height = 16;
    t->sprites[0] = SPRITE_IDX_BOTTOM_BAR;
    t->sprites[1] = 0;
    t->sprite_idx = 0;
    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
}

void tile_create_grail_banner(tile_t* t, int x, int y)
{
    t->x = x;
    t->y = y;
    t->width = 176;
    t->height = 14;
    t->sprites[0] = SPRITE_IDX_GRAIL_BANNER;
    t->sprites[1] = 0;
    t->sprite_idx = 0;
    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
}

int tile_dave_get_sprite(tile_dave_t* dave)
{
    if (dave->mod == TILE_DAVE_MOD_FRONT) {
        return dave->sprite_front;
    } else if (dave->mod == TILE_DAVE_MOD_RIGHT) {
        return dave->sprites_right[dave->walk_cycle_offset];
    } else if (dave->mod == TILE_DAVE_MOD_LEFT) {
        return dave->sprites_left[dave->walk_cycle_offset];
    } else if (dave->mod == TILE_DAVE_MOD_JUMPING_RIGHT) {
        return dave->sprite_jump_right;
    } else if (dave->mod == TILE_DAVE_MOD_JUMPING_LEFT) {
        return dave->sprite_jump_left;
    }

    return -1;
}

void tile_create_purple_gem(tile_t *t, int x, int y)
{
    t->x = x;
    t->y = y;
    t->width = 16;
    t->height = 16;
    t->sprites[0] = SPRITE_IDX_PURPLE_GEM;
    t->sprites[1] = 0;
    t->sprite_idx = 0;
    t->mod = ITEM;

    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
    t->is_inside = &tile_is_inside;
}

void tile_create_teal_gem(tile_t *t, int x, int y)
{
    t->x = x;
    t->y = y;
    t->width = 16;
    t->height = 16;
    t->sprites[0] = SPRITE_IDX_TEAL_GEM;
    t->sprites[1] = 0;
    t->sprite_idx = 0;
    t->mod = ITEM;

    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
    t->is_inside = &tile_is_inside;
}

void tile_create_grail(tile_t *t, int x, int y)
{
    t->x = x;
    t->y = y;
    t->width = 16;
    t->height = 16;

    t->sprites[0] = t->sprites[1] = t->sprites[2] = t->sprites[3] =
    t->sprites[4] = t->sprites[5] = t->sprites[6] = t->sprites[7] = 
    t->sprites[8] = t->sprites[9] = t->sprites[10] = t->sprites[11] = SPRITE_IDX_GRAIL0;

    t->sprites[12] = t->sprites[13] = t->sprites[14] = t->sprites[15] =
    t->sprites[16] = t->sprites[17] = t->sprites[18] = t->sprites[19] =
    t->sprites[20] = t->sprites[21] = t->sprites[22] = t->sprites[23] = SPRITE_IDX_GRAIL1;

    t->sprites[24] = t->sprites[25] = t->sprites[26] = t->sprites[27] =
    t->sprites[28] = t->sprites[29] = t->sprites[30] = t->sprites[31] =
    t->sprites[32] = t->sprites[33] = t->sprites[34] = t->sprites[35] = SPRITE_IDX_GRAIL2;

    t->sprites[36] = t->sprites[37] = t->sprites[38] = t->sprites[39] =
    t->sprites[40] = t->sprites[41] = t->sprites[42] = t->sprites[43] =
    t->sprites[44] = t->sprites[45] = t->sprites[46] = t->sprites[47] = SPRITE_IDX_GRAIL3;

    t->sprites[48] = t->sprites[49] = t->sprites[50] = t->sprites[51] =
    t->sprites[52] = t->sprites[53] = t->sprites[54] = t->sprites[55] =
    t->sprites[56] = t->sprites[57] = t->sprites[58] = t->sprites[59] = SPRITE_IDX_GRAIL4;

    t->sprites[60] = 0;
    t->mod = ITEM;

    t->get_sprite = &tile_get_sprite;
    t->tick = &tile_tick;
    t->is_inside = &tile_is_inside;
}

void tile_create(tile_t* t, char tag[4], int x, int y)
{
    printf("CREATE TILE AT %d, %d \n", x, y);
    if (strcmp(tag, "RBK") == 0) {
        tile_create_block(t, SPRITE_IDX_RED_BRICK, x, y, 16, 16);
    } else if (strcmp(tag, "DRT") == 0) {
        tile_create_block(t, SPRITE_IDX_DIRT, x, y, 16, 16);
    } else if (strcmp(tag, "PIR") == 0) {
        tile_create_block(t, SPRITE_IDX_PIPE_RIGHT, x, y, 16, 16);
    } else if (strcmp(tag, " X ") == 0) {
        tile_create_block(t, SPRITE_IDX_DOOR, x, y, 16, 16);
    } else if (strcmp(tag, " * ") == 0) {
        tile_create_purple_gem(t, x, y);
    } else if (strcmp(tag, " v ") == 0) {
        tile_create_teal_gem(t, x, y);
    } else if (strcmp(tag, " Y ") == 0) {
        tile_create_grail(t, x, y);
    }
}

int tile_file_parse(tile_t* map, const char* path)
{
    long fsize;
    char *buf;
    FILE* f = fopen("res/levels/level0.ndt", "rb");
    fseek(f, 0, SEEK_END);
    fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    buf = malloc(fsize + 1);
    fread(buf, 1, fsize, f);
    fclose(f);

    buf[fsize] = 0;

    return tile_map_parse(map, buf);
}

int tile_map_parse(tile_t* map, char* map_str)
{
    int cur_row = 0;
    int pos = 0;

    int i = 0;
    int in_comment = 0;
    int collected_count = 0;
    char tag[4];
    tag[3] = 0;

    while (map_str[i] != 0) {
        if (in_comment) {
            if (map_str[i] == '\n') {
                in_comment = 0;
            }
        } else {
            if (map_str[i] == '#') {
                in_comment = 1;
            } else if (map_str[i] == ',') {
                if (collected_count == 3) {
                    printf("%s,", tag);
                    collected_count = 0;
                    tile_create(&map[cur_row*20 + pos], tag, pos*16, cur_row*16);
                    pos++;
                } else {
                    return -1;
                }
            } else if (map_str[i] == ';') {
                if (collected_count == 3) {
                    printf("%s;\n", tag);
                    collected_count = 0;
                    tile_create(&map[cur_row*20 + pos], tag, pos*16, cur_row*16);
                    cur_row++;
                    pos = 0;
                } else {
                    return -2;
                }
            } else if (map_str[i] == '\n') {
                //just ignore
            } else {
                if (collected_count >= 3) {
                    return -3;
                }
                tag[collected_count] = map_str[i];
                collected_count++;
            }
        }
        i++;
    } // while

    return 0;
}


