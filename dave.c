#include <stdio.h>
#include <stdlib.h>

#include "tile.h"
#include "dave.h"


#define RIGHT 50
#define LEFT  51
#define UP    52
#define DOWN  53


void dave_state_jumping_enter(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT], int key_left, int key_right, int key_up);
void dave_state_freefalling_enter(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT], int key_left, int key_right, int key_up);
void dave_state_standing_enter(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT], int key_left, int key_right, int key_up);

/**
 * Returns 1 if dave tile is collisioned in its right side. 0 otherwise
 */
static int dave_collision_right(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT])
{
    int idx = 0;
    for (idx = 0; idx < TILEMAP_WIDTH * TILEMAP_HEIGHT; idx++) {
        if (map[idx].sprites[0] != 0 && map[idx].mod == BRICK) {
            if(map[idx].is_inside(&map[idx], dave->x+14, dave->y)) {
                return 1;
            }
            if (map[idx].is_inside(&map[idx], dave->x+14, dave->y+15)) {
                return 1;
            }
        }
    }
    return 0;
}

/**
 *  Returns 1 if dave tile is collisioned in its top side. 0 otherwise
 */
static int dave_collision_top(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT])
{
    int idx = 0;
    for (idx = 0; idx < TILEMAP_WIDTH * TILEMAP_HEIGHT; idx++) {
        if (map[idx].sprites[0] != 0 && map[idx].mod == BRICK) {
            if( map[idx].is_inside(&map[idx], dave->x+4, dave->y)) {
                return 1;
            }
            if( map[idx].is_inside(&map[idx], dave->x+9, dave->y)) {
                return 1;
            }
        }
    }
    return 0;
}

/**
 * Returns 1 if dave tile is collisioned in its left side. 0 otherwise
 */
static int dave_collision_left(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT])
{
    int idx = 0;
    for (idx = 0; idx < TILEMAP_WIDTH * TILEMAP_HEIGHT; idx++) {
        if (map[idx].sprites[0] != 0 && map[idx].mod == BRICK) {
            if( map[idx].is_inside(&map[idx], dave->x - 2, dave->y)) {
                return 1;
            }
            if( map[idx].is_inside(&map[idx], dave->x - 2, dave->y + 15)) {
                return 1;
            }
        }
    }
    return 0;
}

/**
 * Returns 1 if dave tile is on solid ground. 0 otherwise
 */
static int dave_on_ground(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT])
{
    int idx = 0;
    for (idx = 0; idx < TILEMAP_WIDTH * TILEMAP_HEIGHT; idx++) {
        if (map[idx].sprites[0] != 0 && map[idx].mod == BRICK) {

            if(map[idx].is_inside(&map[idx], dave->x + 9, dave->y+16)) {
                return 1;
            }
            if(map[idx].is_inside(&map[idx], dave->x + 8, dave->y+16)) {
                return 1;
            }
            if(map[idx].is_inside(&map[idx], dave->x + 7, dave->y+16)) {
                return 1;
            }
            if(map[idx].is_inside(&map[idx], dave->x + 6, dave->y+16)) {
                return 1;
            }
            if(map[idx].is_inside(&map[idx], dave->x + 5, dave->y+16)) {
                return 1;
            }
        }
    }
    return 0;
}

void dave_state_walking_enter(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT], int key_left, int key_right, int key_up)
{
//    printf("WALKING-ENTER\n");

    if (key_left) {
        if (dave_collision_left(dave, map)) {
            dave->state = DAVE_STATE_STANDING;
        } else {
            dave->state = DAVE_STATE_WALKING;
            dave->x-=2;
            dave->face_direction = DAVE_DIRECTION_LEFT;
            dave->walk_state = DAVE_WALKING_STATE_COOLDOWN2_LEFT;
            dave->step_count++;
        }
    } else if (key_right) {

        if (dave_collision_right(dave, map)) {
            dave->state = DAVE_STATE_STANDING;
        } else {
            dave->state = DAVE_STATE_WALKING;
            dave->x+=2;
            dave->face_direction = DAVE_DIRECTION_RIGHT;
            dave->walk_state = DAVE_WALKING_STATE_COOLDOWN2_RIGHT;
            dave->step_count++;
        }
    }

    if (key_up) {
        dave_state_jumping_enter(dave, map, key_left, key_right, key_up);
        return;
    }
}

void dave_state_walking_routine(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT], int key_left, int key_right, int key_up)
{
    //printf("WALKING\n");
    if (!dave_on_ground(dave, map)) {
        dave_state_freefalling_enter(dave, map, key_left, key_right, key_up);
        return;
    }
    if (dave->walk_state == DAVE_WALKING_STATE_COOLDOWN2_RIGHT) {
        dave->walk_state = DAVE_WALKING_STATE_COOLDOWN1_RIGHT;

    } else if (dave->walk_state == DAVE_WALKING_STATE_COOLDOWN2_LEFT) {
        dave->walk_state = DAVE_WALKING_STATE_COOLDOWN1_LEFT;

    } else if (dave->walk_state == DAVE_WALKING_STATE_COOLDOWN1_RIGHT) {
        dave->walk_state = DAVE_WALKING_STATE_STANDING;
        dave_state_standing_enter(dave, map, key_left, key_right, key_up);

    } else if (dave->walk_state == DAVE_WALKING_STATE_COOLDOWN1_LEFT) {
        dave->walk_state = DAVE_WALKING_STATE_STANDING;
        dave_state_standing_enter(dave, map, key_left, key_right, key_up);
    }

}

void dave_state_jumping_enter(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT], int key_left, int key_right, int key_up)
{
//    printf("JUMPING-ENTER\n");
    dave->state = DAVE_STATE_JUMPING;
    dave->jump_state = 1;
}

void dave_state_jumping_routine(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT], int key_left, int key_right, int key_up)
{
    int jump_velocity_table[90] = {
             0,  0,  0, -3, -2,  0, -2, 0, -2, 0,
            -2, -2,  0, -2,  0, -2, -2, 0, -2, 0,
            -2, -1, -1,  0, -1,  0, -1, 0, -1, 0,
            -1,  0, -1,  0, -1,  0, -1, 0,  0, 0,
             0,  0,  0,  0,  0,  0,  0, 0,  0, 0,
             0,  0,  1,  0,  1,  0,  1, 0,  1, 0,
             1,  0,  1,  0,  1,  0,  2, 0,  2, 0,
             2,  0,  2,  0,  2,  0,  2, 0,  2, 0,
             2,  0,  2,  0,  2,  0,  3, 1,  1, 0};

    if (dave->jump_state >= 35 && dave_on_ground(dave, map)) {
        dave_state_standing_enter(dave, map, key_left, key_right, key_up);
        return;
    }

    if (dave->jump_state >= 90) {
        dave_state_freefalling_enter(dave, map, key_left, key_right, key_up);
        return;
    }
    int deltay = jump_velocity_table[dave->jump_state];
    if (deltay > 0) {
        while(deltay > 0) {
            dave->y = dave->y + 1;
            if (dave_on_ground(dave, map)) {
                deltay = 0;
            } else  {
                deltay--;
            }
        }
    } else if (deltay < 0) {
        while(deltay < 0) {
            dave->y = dave->y - 1;

            if (dave_collision_top(dave, map)) {
                deltay = 0;
                dave->jump_state = 88;
            } else {
                deltay++;
            }
        }
    }
    dave->jump_state++;
    if (dave->walk_state == DAVE_WALKING_STATE_COOLDOWN1_LEFT ||
            dave->walk_state == DAVE_WALKING_STATE_COOLDOWN2_LEFT ||
            dave->walk_state == DAVE_WALKING_STATE_COOLDOWN1_RIGHT ||
            dave->walk_state == DAVE_WALKING_STATE_COOLDOWN2_RIGHT) {
        dave->walk_state = DAVE_STATE_STANDING;
     //   return;
    }else {

    if (key_left) {
        if (dave->walk_state == DAVE_STATE_STANDING){
            dave->face_direction = DAVE_DIRECTION_LEFT;
            if (dave_collision_left(dave,map)) {
            } else {
                dave->x-=2;
                dave->walk_state = DAVE_WALKING_STATE_COOLDOWN2_LEFT;
            }

        }
    } else if (key_right) {
        if (dave->walk_state == DAVE_STATE_STANDING) {
            dave->face_direction = DAVE_DIRECTION_RIGHT;
            if (dave_collision_right(dave, map)) {
            } else {
                dave->x+=2;
                dave->walk_state = DAVE_WALKING_STATE_COOLDOWN2_RIGHT;
            }
        }
    }
    }

}

void dave_state_freefalling_routine(dave_t *dave, 
        tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT], int key_left, int key_right, int key_up)
{
//    printf("FREEFALL \n");
    if (dave_on_ground(dave, map)) {
        dave_state_standing_enter(dave, map, key_left, key_right, key_up);

    } else {
        dave->y = dave->y +1;

        if (key_left) {
            dave->freefall_direction = DAVE_DIRECTION_LEFT;
        } else if (key_right) {
            dave->freefall_direction = DAVE_DIRECTION_RIGHT;
        }

        if (dave->freefall_direction == DAVE_DIRECTION_LEFT) {
            if ( !dave_collision_left(dave, map)) {
                dave->x = dave->x - 1;
            }
        } else if (dave->freefall_direction == DAVE_DIRECTION_RIGHT) {
            if ( !dave_collision_right(dave, map)) {
                dave->x = dave->x + 1;
            }
        }
    }
}

void dave_state_freefalling_enter(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT], int key_left, int key_right, int key_up)
{
    dave->state = DAVE_STATE_FREEFALLING;
    dave->freefall_direction = DAVE_DIRECTION_FRONT;
}

void dave_state_standing_enter(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT], int key_left, int key_right, int key_up)
{
//    printf("STANDING-ENTER\n");
    dave->state = DAVE_STATE_STANDING;
}

void dave_state_standing_routine(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT], int key_left, int key_right, int key_up)
{
  //  printf("STANDING\n");
    if (key_up) {
        dave_state_jumping_enter(dave, map, key_left, key_right, key_up);
    }

    if (key_left || key_right) {
        dave_state_walking_enter(dave, map, key_left, key_right, key_up);
    }
}

static void dave_tick(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT], int key_left, int key_right, int key_up)
{
    if (dave->state == DAVE_STATE_STANDING) {
        dave_state_standing_routine(dave, map, key_left, key_right, key_up);
    } else if (dave->state == DAVE_STATE_WALKING) {
        dave_state_walking_routine(dave, map, key_left, key_right, key_up);
    } else if (dave->state == DAVE_STATE_JUMPING) {
        dave_state_jumping_routine(dave, map, key_left, key_right, key_up);
    } else if (dave->state == DAVE_STATE_FREEFALLING) {
        dave_state_freefalling_routine(dave, map, key_left, key_right, key_up);
    }
}

dave_t* dave_create()
{
    dave_t *dave = malloc(sizeof(dave_t));
    dave->step_count = 0;
    dave->walk_state = DAVE_STATE_STANDING;
    dave->freefall_direction = DAVE_DIRECTION_FRONT;
    dave->tick = &dave_tick;
    return dave;
}
