#include <stdio.h>
#include <stdlib.h>

#include "tile.h"
#include "dave.h"


/*
 * Returns 1 if dave tile is collisioned in its right side. 0 otherwise
 */
static int dave_collision_right(dave_t *dave, int delta, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT]) {
    int idx = 0;
    for (idx = 0; idx < TILEMAP_WIDTH * TILEMAP_HEIGHT; idx++) {
        if (map[idx].sprites[0] != 0 && map[idx].mod == BRICK) {
            if(map[idx].is_inside(&map[idx], dave->tile->x + 16 - delta, dave->tile->y+2)) {
                return 1;
            }
            if (map[idx].is_inside(&map[idx], dave->tile->x + 16 - delta, dave->tile->y+15)) {
                return 1;
            }
        }
    }
    return 0;
}

/*
 *  Returns 1 if dave tile is collisioned in its top side. 0 otherwise
 */
static int dave_collision_top(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT]) {
    int idx = 0;
    for (idx = 0; idx < TILEMAP_WIDTH * TILEMAP_HEIGHT; idx++) {
        if (map[idx].sprites[0] != 0 && map[idx].mod == BRICK) {
            if( map[idx].is_inside(&map[idx], dave->tile->x+4, dave->tile->y)) {
                return 1;
            }
            if( map[idx].is_inside(&map[idx], dave->tile->x+9, dave->tile->y)) {
                return 1;
            }
        }
    }
    return 0;
}

/*
 * Returns 1 if dave tile is collisioned in its left side. 0 otherwise
 */
static int dave_collision_left(dave_t *dave, int delta, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT]) {
    int idx = 0;
    for (idx = 0; idx < TILEMAP_WIDTH * TILEMAP_HEIGHT; idx++) {
        if (map[idx].sprites[0] != 0 && map[idx].mod == BRICK) {
            if( map[idx].is_inside(&map[idx], dave->tile->x - delta , dave->tile->y + 2)) {
                return 1;
            }
            if( map[idx].is_inside(&map[idx], dave->tile->x - delta, dave->tile->y + 15)) {
                return 1;
            }
        }
    }
    return 0;
}

/**
 * Returns 1 if dave can make the jump miracously.
 * This mimics original game's forgiving nature of increasing ground collision during freefall
 */ 
static int dave_barely_on_ground_right(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT]) {
    int idx = 0;
    for (idx = 0; idx < TILEMAP_WIDTH * TILEMAP_HEIGHT; idx++) {
        if (map[idx].sprites[0] != 0 && map[idx].mod == BRICK) {
            if(map[idx].is_inside(&map[idx], dave->tile->x + 17, dave->tile->y+16)) {
                return 1;
            }
            if(map[idx].is_inside(&map[idx], dave->tile->x + 17, dave->tile->y+17)) {
                return 1;
            }
            if(map[idx].is_inside(&map[idx], dave->tile->x + 17, dave->tile->y+18)) {
                return 1;
            }
        }
    }
    return 0;
}

/**
 * Returns 1 if dave can make the jump miracously.
 * This mimics original game's forgiving nature of increasing ground collision during freefall
 */ 
static int dave_barely_on_ground_left(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT]) {
    int idx = 0;
    for (idx = 0; idx < TILEMAP_WIDTH * TILEMAP_HEIGHT; idx++) {
        if (map[idx].sprites[0] != 0 && map[idx].mod == BRICK) {
            if( map[idx].is_inside(&map[idx], dave->tile->x+1, dave->tile->y + 16)) {
                return 1;
            }
            if( map[idx].is_inside(&map[idx], dave->tile->x+1, dave->tile->y + 17)) {
                return 1;
            }
        }
    }
    return 0;
}

/*
 * Returns 1 if dave tile is on solid ground. 0 otherwise
 */
static int dave_on_ground(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT]) {
    int idx = 0;
    for (idx = 0; idx < TILEMAP_WIDTH * TILEMAP_HEIGHT; idx++) {
        if (map[idx].sprites[0] != 0 && map[idx].mod == BRICK) {

            /* This will allow climbing on right walls
            if(map[idx].is_inside(&map[idx], dave->tile->x + 10, dave->tile->y+16)) {
                return 1;
            }
            */

            if(map[idx].is_inside(&map[idx], dave->tile->x + 9, dave->tile->y+16)) {
                return 1;
            }
            if(map[idx].is_inside(&map[idx], dave->tile->x + 8, dave->tile->y+16)) {
                return 1;
            }
            if(map[idx].is_inside(&map[idx], dave->tile->x + 7, dave->tile->y+16)) {
                return 1;
            }
            if(map[idx].is_inside(&map[idx], dave->tile->x + 6, dave->tile->y+16)) {
                return 1;
            }
            if(map[idx].is_inside(&map[idx], dave->tile->x + 5, dave->tile->y+16)) {
                return 1;
            }

            //TODO: Is this needed?
            if(map[idx].is_inside(&map[idx], dave->tile->x + 4, dave->tile->y+16)) {
                return 1;
            }
        }
    }
    return 0;
}

int delayer = 0;

void dave_state_burning_enter(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
        int key_left, int key_right, int key_up) {
    printf("BURNING-ENTER\n");
    dave->state = DAVE_STATE_BURNING;
    dave->ticks_since_dead = 0;
}

void dave_state_standing_enter(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
    int key_left, int key_right, int key_up) {
    printf("STANDING ENTER DIRECTION: %d \n", dave->freefall_direction);
    dave->state = DAVE_STATE_STANDING;
}

void dave_state_jumping_enter(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
        int key_left, int key_right, int key_up) {
    printf("JUMPING-ENTER\n");
    if (dave->face_direction == DAVE_DIRECTION_FRONTL ||
        dave->face_direction == DAVE_DIRECTION_LEFT) {
        dave->face_direction = DAVE_DIRECTION_FRONTL;
    } else {
        dave->face_direction = DAVE_DIRECTION_FRONTR;
    }
    dave->state = DAVE_STATE_JUMPING;
    dave->walk_state = DAVE_STATE_STANDING;
    dave->jump_state = 1;
//    dave->step_count = 0;

/*        if (key_left) {
            if (dave->walk_state == DAVE_STATE_STANDING){
                dave->face_direction = DAVE_DIRECTION_LEFT;
                if (dave_collision_left(dave, 2, map)) {
                } else {
                    dave->tile->x-=2;
                    dave->walk_state = DAVE_WALKING_STATE_COOLDOWN2_LEFT;
                }
            }
        } else if (key_right) {
            if (dave->walk_state == DAVE_STATE_STANDING) {
                dave->face_direction = DAVE_DIRECTION_RIGHT;
                if (dave_collision_right(dave, 2, map)) {
                } else {
                    dave->tile->x+=2;
                    dave->walk_state = DAVE_WALKING_STATE_COOLDOWN2_RIGHT;
                }
            }
        }*/
}

void dave_state_walking_enter(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
        int key_left, int key_right, int key_up) {
    printf("WALKING-ENTER\n");
    if (key_left) {
        if (dave_collision_left(dave, 2, map)) {
            dave->state = DAVE_STATE_STANDING;
        } else {
            dave->state = DAVE_STATE_WALKING;
            dave->tile->x-=2;
            dave->face_direction = DAVE_DIRECTION_LEFT;
            dave->walk_state = DAVE_WALKING_STATE_COOLDOWN2_LEFT;
            dave->step_count++;
        }
    } else if (key_right) {

        if (dave_collision_right(dave, 2, map)) {
            dave->state = DAVE_STATE_STANDING;
        } else {
            dave->state = DAVE_STATE_WALKING;
            dave->tile->x+=2;
            dave->face_direction = DAVE_DIRECTION_RIGHT;
            dave->walk_state = DAVE_WALKING_STATE_COOLDOWN2_RIGHT;
            dave->step_count++;
        }
    }

    if (delayer > 0) {
        delayer--;
    }

    if (key_up) {
        if (delayer <= 0) {
            dave_state_jumping_enter(dave, map, key_left, key_right, key_up);
        }
    }
}

void dave_state_jetpacking_routine(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
        int key_left, int key_right, int key_up, int key_down, int key_jetpack) {

    printf("JETPACKING ROUTINE\n");

    if (key_jetpack) {
        dave_state_standing_enter(dave, map, key_left, key_right, key_up);
        return;
    }

    if (key_left) {
        if (!dave_collision_left(dave, 1, map)) {
            dave->tile->x-=1;
            dave->face_direction = DAVE_DIRECTION_LEFT;
        }
    } else if (key_right) {
        if (!dave_collision_right(dave, 1, map)) {
            dave->tile->x+=1;
            dave->face_direction = DAVE_DIRECTION_RIGHT;
        }
    } else if (key_up) {
        if (!dave_collision_top(dave, map)) {
            dave->tile->y-=1;
        }
    } else if (key_down) {
        if (!dave_on_ground(dave, map)) {
            dave->tile->y+=1;
        }
    }
    dave->ticks_in_state++;
/*    if (dave->is_dead) {
        dave_state_burning_enter(dave, map, key_left, key_right, key_up);
        return;
    }

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
    }*/
}

void dave_state_jetpacking_enter(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
        int key_left, int key_right, int key_up) {
    printf("JETPACKING ENTER\n");
    dave->state = DAVE_STATE_JETPACKING;
    dave->ticks_in_state = 0;
/*    if (key_left) {
        if (dave_collision_left(dave, map)) {
            dave->state = DAVE_STATE_STANDING;
        } else {
            dave->state = DAVE_STATE_WALKING;
            dave->tile->x-=2;
            dave->face_direction = DAVE_DIRECTION_LEFT;
            dave->walk_state = DAVE_WALKING_STATE_COOLDOWN2_LEFT;
            dave->step_count++;
        }
    } else if (key_right) {

        if (dave_collision_right(dave, map)) {
            dave->state = DAVE_STATE_STANDING;
        } else {
            dave->state = DAVE_STATE_WALKING;
            dave->tile->x+=2;
            dave->face_direction = DAVE_DIRECTION_RIGHT;
            dave->walk_state = DAVE_WALKING_STATE_COOLDOWN2_RIGHT;
            dave->step_count++;
        }
    }

        if (delayer > 0) {
            delayer--;
        }
    if (key_up) {
        if (delayer > 0) {
         //   delayer--;
        } else {
            dave_state_jumping_enter(dave, map, key_left, key_right, key_up);
        }
        return;
    }*/
}
void dave_state_freefalling_enter(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
    int key_left, int key_right, int key_up) {

    dave->state = DAVE_STATE_FREEFALLING;

    if (dave->face_direction == DAVE_DIRECTION_RIGHT ||
        dave->face_direction == DAVE_DIRECTION_FRONTR) {
        dave->freefall_direction = DAVE_DIRECTION_FRONTR;

    } else if (dave->face_direction == DAVE_DIRECTION_LEFT ||
        dave->face_direction == DAVE_DIRECTION_FRONTL) {
        dave->freefall_direction = DAVE_DIRECTION_FRONTL;
    }
}


void dave_state_burning_routine(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
        int key_left, int key_right, int key_up, int key_jetpack) {
    dave->ticks_since_dead++;
}

void dave_state_walking_routine(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
        int key_left, int key_right, int key_up, int key_jetpack) {

    if (dave->is_dead) {
        dave_state_burning_enter(dave, map, key_left, key_right, key_up);
        return;
    }

    if (key_jetpack) {
        dave_state_jetpacking_enter(dave, map, key_left, key_right, key_up);
        return;
    }

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

void dave_state_jumping_routine(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
        int key_left, int key_right, int key_up, int key_jetpack) {

    printf("JUMPING ROUTINE \n");
    if (dave->is_dead) {
        dave_state_burning_enter(dave, map, key_left, key_right, key_up);
        return;
    }

    if (key_jetpack) {
        dave_state_jetpacking_enter(dave, map, key_left, key_right, key_up);
        return;
    }
    int jump_velocity_table[94] = {
            -1,  0, -3,  0, -3,  0, -2,  0, -2,  0,
            -2,  0, -2,  0, -2,  0, -2,  0, -2,  0,
            -2,  0, -1,  0, -2,  0, -1,  0, -1,  0,
            -1,  0, -1,  0, -1,  0, -1,  0, -1,  0, //40 total up

             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
             0,  0,  0,  0,// 14 total hang

             0,  0,  1,  0,  1,  0,  1,  0,  1,  0, //40 total down
             1,  0,  1,  0,  1,  0,  2,  0,  2,  0,
             2,  0,  2,  0,  2,  0,  2,  0,  2,  0,
             2,  0,  2,  0,  2,  0,  3,  1,  1,  0};

    if (dave->jump_state >= 40 && dave_on_ground(dave, map)) {
        dave_state_standing_enter(dave, map, key_left, key_right, key_up);
        delayer = 5;
        dave->step_count = 5;
        return;
    }

    if (dave->jump_state >= 94) {
        if (key_left) {
                if (dave_collision_left(dave, 1, map)) {
                //dave->tile->x-=2;
                } else {
                    dave->tile->x-=1;
//                    dave->walk_state = DAVE_WALKING_STATE_COOLDOWN2_LEFT;
                }
        } else if (key_right) {
            if (dave_collision_right(dave, 1, map)) {
               // printf("WTF RIGHT \n");
               // dave->tile->x+=2;
            } else {
                dave->tile->x+=1;
  //              dave->walk_state = DAVE_WALKING_STATE_COOLDOWN2_RIGHT;
            }
        }
        dave->tile->y = dave->tile->y + 1;
        dave_state_freefalling_enter(dave, map, key_left, key_right, key_up);
        return;
    }

    int deltay = jump_velocity_table[dave->jump_state];
    if (deltay > 0) {
        while(deltay > 0) {
            dave->tile->y = dave->tile->y + 1;
            if (dave_on_ground(dave, map)) {
                deltay = 0;
            } else  {
                deltay--;
            }
        }
    } else if (deltay < 0) {
        while(deltay < 0) {
            dave->tile->y = dave->tile->y - 1;

            if (dave_collision_top(dave, map)) {
                dave->walk_state = DAVE_STATE_STANDING;
                if (dave->face_direction == DAVE_DIRECTION_RIGHT) {
                    printf("right \n");
                    dave->freefall_direction = DAVE_DIRECTION_FRONTR;
                    dave->face_direction = DAVE_DIRECTION_FRONTR;
                } else if (dave->face_direction == DAVE_DIRECTION_LEFT) {
                    printf("left \n");
                    dave->freefall_direction = DAVE_DIRECTION_FRONTL;
                    dave->face_direction = DAVE_DIRECTION_FRONTL;
                }
               /* if (dave->face_direction == DAVE_DIRECTION_RIGHT) {
                    dave->face_direction = DAVE_DIRECTION_FRONTR;
                } else if (dave->face_direction == DAVE_DIRECTION_LEFT) {
                    dave->face_direction = DAVE_DIRECTION_FRONTL;
                }*/
//                dave->tile->y = dave->tile->y - 1;
                deltay = 0;
                dave->jump_state = 94;
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
    } else {

        if (key_left) {
            if (dave->walk_state == DAVE_STATE_STANDING){
                dave->face_direction = DAVE_DIRECTION_LEFT;
                if (dave_collision_left(dave, 2, map)) {
                } else {
                    dave->tile->x-=2;
                    dave->walk_state = DAVE_WALKING_STATE_COOLDOWN2_LEFT;
                }
            }
        } else if (key_right) {
            if (dave->walk_state == DAVE_STATE_STANDING) {
                dave->face_direction = DAVE_DIRECTION_RIGHT;
                if (dave_collision_right(dave, 2, map)) {
                } else {
                    dave->tile->x+=2;
                    dave->walk_state = DAVE_WALKING_STATE_COOLDOWN2_RIGHT;
                }
            }
        }
    }
}

void dave_state_freefalling_routine(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
    int key_left, int key_right, int key_up, int key_jetpack) {

    if (dave->is_dead) {
        dave_state_burning_enter(dave, map, key_left, key_right, key_up);
        return;
    }

    if (key_jetpack) {
        dave_state_jetpacking_enter(dave, map, key_left, key_right, key_up);
        return;
    }

    if (dave_on_ground(dave, map)) {
        dave_state_standing_enter(dave, map, key_left, key_right, key_up);
        delayer = 5;
        dave->step_count = 5;
    } else {

        dave->tile->y = dave->tile->y + 1;
        if (key_left) {
            dave->freefall_direction = DAVE_DIRECTION_LEFT;
            dave->face_direction = DAVE_DIRECTION_LEFT;
        } else if (key_right) {
            dave->freefall_direction = DAVE_DIRECTION_RIGHT;
            dave->face_direction = DAVE_DIRECTION_RIGHT;
        }

        if (dave->freefall_direction == DAVE_DIRECTION_LEFT) {
            if ( !dave_collision_left(dave, 1, map)) {
                dave->tile->x = dave->tile->x - 1;
                if (key_left) {
                    if (dave_barely_on_ground_left(dave, map)) {
                        dave->tile->x = dave->tile->x - 1;
                    }
                }
            }
        } else if (dave->freefall_direction == DAVE_DIRECTION_RIGHT) {
            if ( !dave_collision_right(dave, 1, map)) {
                dave->tile->x = dave->tile->x + 1;
                if (key_right) {
                    if (dave_barely_on_ground_right(dave, map)) {
                        dave->tile->x = dave->tile->x + 1;
                    }
                }
            }
        }

       /* if (dave_on_ground(dave, map)) {
            return;
        }*/
    }
}

void dave_state_standing_routine(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
    int key_left, int key_right, int key_up, int key_jetpack) {

    if (dave->is_dead) {
        dave_state_burning_enter(dave, map, key_left, key_right, key_up);
        return;
    }

    if (key_jetpack) {
        dave_state_jetpacking_enter(dave, map, key_left, key_right, key_up);
        return;
    }

    if (!dave_on_ground(dave, map)) {
        dave_state_freefalling_enter(dave, map, key_left, key_right, key_up);
        return;
    }


    if (delayer > 0) {
        delayer = delayer -1;
    }
    if (key_up) {
        if (delayer > 0) {
     //       delayer = delayer -1;
        } else {
            dave_state_jumping_enter(dave, map, key_left, key_right, key_up);
        }
        return;
    }

    if (key_left || key_right) {
        dave_state_walking_enter(dave, map, key_left, key_right, key_up);
        return;
    }
}

static void dave_tick(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
        int key_left, int key_right, int key_up, int key_down, int key_jetpack) {

    if (dave->state == DAVE_STATE_STANDING) {
        dave_state_standing_routine(dave, map, key_left, key_right, key_up, key_jetpack);
    } else if (dave->state == DAVE_STATE_WALKING) {
        dave_state_walking_routine(dave, map, key_left, key_right, key_up, key_jetpack);
    } else if (dave->state == DAVE_STATE_JUMPING) {
        dave_state_jumping_routine(dave, map, key_left, key_right, key_up, key_jetpack);
    } else if (dave->state == DAVE_STATE_FREEFALLING) {
        dave_state_freefalling_routine(dave, map, key_left, key_right, key_up, key_jetpack);
    } else if (dave->state == DAVE_STATE_BURNING) {
        dave_state_burning_routine(dave, map, key_left, key_right, key_up, key_jetpack);
    } else if (dave->state == DAVE_STATE_JETPACKING) {
        dave_state_jetpacking_routine(dave, map, key_left, key_right, key_up, key_down, key_jetpack);
    }
}

int dave_get_sprite(tile_t *tile) {
    int sprite;
    int walk_mod;

    dave_t *dave = (dave_t *)tile->context;
    walk_mod = dave->step_count%8;

    if (dave->state == DAVE_STATE_FREEFALLING) {
        if (dave->freefall_direction == DAVE_DIRECTION_LEFT) {
            sprite = SPRITE_IDX_DAVE_LEFT_HANDSFREE;
        } else if (dave->freefall_direction == DAVE_DIRECTION_RIGHT) {
            sprite = SPRITE_IDX_DAVE_RIGHT_HANDSFREE;
        } else if (dave->freefall_direction == DAVE_DIRECTION_FRONTR) {
            sprite = SPRITE_IDX_DAVE_JUMP_RIGHT;
        } else if (dave->freefall_direction == DAVE_DIRECTION_FRONTL) {
            sprite = SPRITE_IDX_DAVE_JUMP_LEFT;
        } else {
            sprite = SPRITE_IDX_DAVE_FRONT;
        }
    } else if (dave->state == DAVE_STATE_JUMPING) {
        if (dave->face_direction == DAVE_DIRECTION_LEFT ||
                dave->face_direction == DAVE_DIRECTION_FRONTL) {
            sprite = SPRITE_IDX_DAVE_JUMP_LEFT;
        } else {
            sprite = SPRITE_IDX_DAVE_JUMP_RIGHT;
        }
    } else if (dave->state == DAVE_STATE_WALKING || dave->state == DAVE_STATE_STANDING) {
        if (dave->face_direction == DAVE_DIRECTION_RIGHT) {
            if (walk_mod == 0 || walk_mod == 1 || walk_mod == 4 || walk_mod == 5) {
                sprite = SPRITE_IDX_DAVE_RIGHT_STAND;
            } else if (walk_mod == 2 || walk_mod == 3) {
                sprite = SPRITE_IDX_DAVE_RIGHT_HANDSFREE;
            } else if (walk_mod == 6 || walk_mod == 7) {
                sprite = SPRITE_IDX_DAVE_RIGHT_SERIOUS;
            } else {
                sprite = 0;
            }
        } else if (dave->face_direction == DAVE_DIRECTION_LEFT) {
            if (walk_mod == 0 || walk_mod == 1 || walk_mod == 4 || walk_mod == 5) {
                sprite = SPRITE_IDX_DAVE_LEFT_STAND;
            } else if (walk_mod == 2 || walk_mod == 3) {
                sprite = SPRITE_IDX_DAVE_LEFT_HANDSFREE;
            } else if (walk_mod == 6 || walk_mod == 7) {
                sprite = SPRITE_IDX_DAVE_LEFT_SERIOUS;
            } else {
                sprite = 0;
            }
        } else {
            sprite = SPRITE_IDX_DAVE_FRONT;
        }
    } else if (dave->state == DAVE_STATE_BURNING) {
        if ((dave->ticks_since_dead / 30) % 4 == 0) {
            sprite = SPRITE_IDX_EXPLOSION1;
        } else if ((dave->ticks_since_dead / 30) % 4 == 1) {
            sprite = SPRITE_IDX_EXPLOSION2;
        } else if ((dave->ticks_since_dead / 30) % 4 == 2) {
            sprite = SPRITE_IDX_EXPLOSION3;
        } else {
            sprite = SPRITE_IDX_EXPLOSION4;
        }
    } else if (dave->state == DAVE_STATE_JETPACKING) {
        if (dave->face_direction == DAVE_DIRECTION_LEFT) {
            if (dave->ticks_in_state % 3 == 0) {
                sprite = SPRITE_IDX_DAVE_JETPACK_LEFT1;
            } else if (dave->ticks_in_state % 3 == 1) {
                sprite = SPRITE_IDX_DAVE_JETPACK_LEFT2;
            } else if (dave->ticks_in_state % 3 == 2) {
                sprite = SPRITE_IDX_DAVE_JETPACK_LEFT3;
            }
        }
        else {//if (dave->face_direction == DAVE_DIRECTION_RIGHT) {
            if (dave->ticks_in_state % 3 == 0) {
                sprite = SPRITE_IDX_DAVE_JETPACK_RIGHT1;
            } else if (dave->ticks_in_state % 3 == 1) {
                sprite = SPRITE_IDX_DAVE_JETPACK_RIGHT2;
            } else if (dave->ticks_in_state % 3 == 2) {
                sprite = SPRITE_IDX_DAVE_JETPACK_RIGHT3;
            }
        }
    } else {
        sprite = SPRITE_IDX_DAVE_FRONT;
    }

    return sprite;
}

dave_t* dave_create() {
    dave_t *dave = malloc(sizeof(dave_t));
    dave->step_count = 0;
    dave->walk_state = DAVE_STATE_STANDING;
    dave->freefall_direction = DAVE_DIRECTION_FRONTR;
    dave->has_trophy = 0;
    dave->is_dead = 0;
    dave->ticks_since_dead = 0;
    dave->ticks_in_state = 0;
    dave->tick = &dave_tick;

    dave->tile = malloc(sizeof(tile_t));
    dave->tile->x = 0;
    dave->tile->y = 0;
    dave->tile->width = 0;
    dave->tile->height = 0;
    dave->tile->mod = DAVE;
    dave->tile->tick = NULL;
    dave->tile->is_inside = NULL;
    dave->tile->context = dave;
    dave->tile->get_sprite = &dave_get_sprite;

    return dave;
}
