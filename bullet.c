#include <stdio.h>
#include <stdlib.h>

#include "tile.h"
#include "dave_bullet.h"


/*
 * Returns 1 if dave tile is collisioned in its right side. 0 otherwise
 */
static int dave_collision_right(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT]) {
    int idx = 0;
    for (idx = 0; idx < TILEMAP_WIDTH * TILEMAP_HEIGHT; idx++) {
        if (map[idx].sprites[0] != 0 && map[idx].mod == BRICK) {
            if(map[idx].is_inside(&map[idx], dave->tile->x+14, dave->tile->y+1)) {
                return 1;
            }
            if (map[idx].is_inside(&map[idx], dave->tile->x+14, dave->tile->y+15)) {
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
static int dave_collision_left(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT]) {
    int idx = 0;
    for (idx = 0; idx < TILEMAP_WIDTH * TILEMAP_HEIGHT; idx++) {
        if (map[idx].sprites[0] != 0 && map[idx].mod == BRICK) {
            if( map[idx].is_inside(&map[idx], dave->tile->x - 2, dave->tile->y + 1)) {
                return 1;
            }
            if( map[idx].is_inside(&map[idx], dave->tile->x - 2, dave->tile->y + 15)) {
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

void dave_state_jumping_enter(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
        int key_left, int key_right, int key_up) {
    printf("JUMPING-ENTER\n");
    dave->state = DAVE_STATE_JUMPING;
    dave->walk_state = DAVE_STATE_STANDING;
    dave->jump_state = 1;
    dave->step_count = 0;

        if (key_left) {
            if (dave->walk_state == DAVE_STATE_STANDING){
                dave->face_direction = DAVE_DIRECTION_LEFT;
                if (dave_collision_left(dave,map)) {
                } else {
                    dave->tile->x-=2;
                    dave->walk_state = DAVE_WALKING_STATE_COOLDOWN2_LEFT;
                }
            }
        } else if (key_right) {
            if (dave->walk_state == DAVE_STATE_STANDING) {
                dave->face_direction = DAVE_DIRECTION_RIGHT;
                if (dave_collision_right(dave, map)) {
                } else {
                    dave->tile->x+=2;
                    dave->walk_state = DAVE_WALKING_STATE_COOLDOWN2_RIGHT;
                }
            }
        }
}

void dave_state_walking_enter(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
        int key_left, int key_right, int key_up) {
    printf("WALKING-ENTER\n");
    if (key_left) {
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
    }
}

void dave_state_freefalling_enter(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
    int key_left, int key_right, int key_up) {
    printf("FREEFALLING ENTER \n");
    dave->state = DAVE_STATE_FREEFALLING;
    if (dave->face_direction == DAVE_DIRECTION_RIGHT) {
        dave->freefall_direction = DAVE_DIRECTION_FRONTR;
    } else if (dave->face_direction == DAVE_DIRECTION_LEFT) {
        dave->freefall_direction = DAVE_DIRECTION_FRONTL;
    }
}

void dave_state_standing_enter(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
    int key_left, int key_right, int key_up) {
    printf("STANDING ENTER DIRECTION: %d \n", dave->freefall_direction);
    dave->state = DAVE_STATE_STANDING;
}

void dave_state_burning_routine(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
        int key_left, int key_right, int key_up) {
    printf("BURNING ROUTINE \n");
    dave->ticks_since_dead++;
}

void dave_state_walking_routine(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
        int key_left, int key_right, int key_up) {

    if (dave->is_dead) {
        dave_state_burning_enter(dave, map, key_left, key_right, key_up);
        return;
    }

    printf("WALKING\n");
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
        int key_left, int key_right, int key_up) {

    if (dave->is_dead) {
        dave_state_burning_enter(dave, map, key_left, key_right, key_up);
        return;
    }

    //printf("JUMP ROUTINE  jumpstate: %d\n", dave->jump_state);
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

    if (dave->jump_state >= 35 && dave_on_ground(dave, map)) {
        dave_state_standing_enter(dave, map, key_left, key_right, key_up);
        delayer = 4;
        return;
    }

    if (dave->jump_state >= 94) {
        printf("XI \n");
        if (key_left) {
            printf("XI key left \n");
                if (dave_collision_left(dave,map)) {
                //printf("WTF LEFT \n");
                //dave->tile->x-=2;
                } else {
            printf("XI LEFT\n");
                    dave->tile->x-=1;
//                    dave->walk_state = DAVE_WALKING_STATE_COOLDOWN2_LEFT;
                }
        } else if (key_right) {
            printf("XI key right \n");
            if (dave_collision_right(dave, map)) {
               // printf("WTF RIGHT \n");
               // dave->tile->x+=2;
            } else {
            printf("XI RIGHT\n");
                dave->tile->x+=1;
  //              dave->walk_state = DAVE_WALKING_STATE_COOLDOWN2_RIGHT;
            }
        }
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
                if (dave_collision_left(dave,map)) {
                } else {
                    dave->tile->x-=2;
                    dave->walk_state = DAVE_WALKING_STATE_COOLDOWN2_LEFT;
                }
            }
        } else if (key_right) {
            if (dave->walk_state == DAVE_STATE_STANDING) {
                dave->face_direction = DAVE_DIRECTION_RIGHT;
                if (dave_collision_right(dave, map)) {
                } else {
                    dave->tile->x+=2;
                    dave->walk_state = DAVE_WALKING_STATE_COOLDOWN2_RIGHT;
                }
            }
        }
    }

}

void dave_state_freefalling_routine(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
    int key_left, int key_right, int key_up) {
    printf("FREEFALL face: %d, freefall: %d\n", dave->face_direction, dave->freefall_direction);

    if (dave->is_dead) {
        dave_state_burning_enter(dave, map, key_left, key_right, key_up);
        return;
    }

    if (dave_on_ground(dave, map)) {
        dave_state_standing_enter(dave, map, key_left, key_right, key_up);
        delayer = 4;

    } else {

        dave->tile->y = dave->tile->y + 1;
        if (dave_on_ground(dave, map)) {
            return;
        }
        if (key_left) {
            dave->freefall_direction = DAVE_DIRECTION_LEFT;
            dave->face_direction = DAVE_DIRECTION_LEFT;
        } else if (key_right) {
            dave->freefall_direction = DAVE_DIRECTION_RIGHT;
            dave->face_direction = DAVE_DIRECTION_RIGHT;
        }

        if (dave->freefall_direction == DAVE_DIRECTION_LEFT) {
            if ( !dave_collision_left(dave, map)) {
                dave->tile->x = dave->tile->x - 1;
            }
        } else if (dave->freefall_direction == DAVE_DIRECTION_RIGHT) {
            if ( !dave_collision_right(dave, map)) {
                dave->tile->x = dave->tile->x + 1;
            }
        }
    }
}



void dave_state_standing_routine(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
    int key_left, int key_right, int key_up) {

    if (dave->is_dead) {
        dave_state_burning_enter(dave, map, key_left, key_right, key_up);
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
        int key_left, int key_right, int key_up) {

    if (dave->state == DAVE_STATE_STANDING) {
        dave_state_standing_routine(dave, map, key_left, key_right, key_up);
    } else if (dave->state == DAVE_STATE_WALKING) {
        dave_state_walking_routine(dave, map, key_left, key_right, key_up);
    } else if (dave->state == DAVE_STATE_JUMPING) {
        dave_state_jumping_routine(dave, map, key_left, key_right, key_up);
    } else if (dave->state == DAVE_STATE_FREEFALLING) {
        dave_state_freefalling_routine(dave, map, key_left, key_right, key_up);
    } else if (dave->state == DAVE_STATE_BURNING) {
        dave_state_burning_routine(dave, map, key_left, key_right, key_up);
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
    } else {
        sprite = SPRITE_IDX_DAVE_FRONT;
    }

    return sprite;
}

dave_bullet_t* dave_bullet_create() {
    dave_bullet_t *bullet = malloc(sizeof(dave_bullet_t));
    bullet->speed_x = 0;
    bullet->direction = DAVE_BULLET_DIR_RIGHT;
    bullet->valid = 0;

    bullet->tick = &dave_bullet_tick;

    bullet->tile = malloc(sizeof(tile_t));
    bullet->tile->x = 0;
    bullet->tile->y = 0;
    bullet->tile->width = 0;
    bullet->tile->height = 0;
    bullet->tile->tick = 0;
    dave->tile->tick = NULL;
    dave->tile->is_inside = NULL;
    dave->tile->context = dave;
    dave->tile->get_sprite = &dave_get_sprite;

    return dave;
}
