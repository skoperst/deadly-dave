#include <stdio.h>
#include <stdlib.h>

#include "dave.h"


/*
 * Returns 1 if dave tile is collisioned in its right side. 0 otherwise
 */
static int dave_collision_right(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT]) {
    int idx = 0;
    for (idx = 0; idx < TILEMAP_WIDTH * TILEMAP_HEIGHT; idx++) {
        if (map[idx].sprites[0] != 0 && map[idx].mod == BRICK) {
            if (map[idx].is_inside(&map[idx], dave->tile->x + 12, dave->tile->y + 2)) {
                return 1;
            }
            if (map[idx].is_inside(&map[idx], dave->tile->x + 12, dave->tile->y + 15)) {
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
            if (map[idx].is_inside(&map[idx], dave->tile->x + 4, dave->tile->y + 1)) {
                return 1;
            }
            if (map[idx].is_inside(&map[idx], dave->tile->x + 9, dave->tile->y + 1)) {
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
            if (map[idx].is_inside(&map[idx], dave->tile->x + 1, dave->tile->y + 2)) {
                return 1;
            }
            if (map[idx].is_inside(&map[idx], dave->tile->x + 1, dave->tile->y + 15)) {
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

            /* This will allow climbing on right walls (original game's feature/bug)
             * if(map[idx].is_inside(&map[idx], dave->tile->x + 10, dave->tile->y+16)) {
             *   return 1;
             * }
            */

            if (map[idx].is_inside(&map[idx], dave->tile->x + 9, dave->tile->y+16)) {
                return 1;
            }
            if (map[idx].is_inside(&map[idx], dave->tile->x + 8, dave->tile->y+16)) {
                return 1;
            }
            if (map[idx].is_inside(&map[idx], dave->tile->x + 7, dave->tile->y+16)) {
                return 1;
            }
            if (map[idx].is_inside(&map[idx], dave->tile->x + 6, dave->tile->y+16)) {
                return 1;
            }
            if (map[idx].is_inside(&map[idx], dave->tile->x + 5, dave->tile->y+16)) {
                return 1;
            }
            if (map[idx].is_inside(&map[idx], dave->tile->x + 4, dave->tile->y+16)) {
                return 1;
            }
        }
    }
    return 0;
}

void dave_state_burning_enter(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
        int key_left, int key_right, int key_up) {
    dave->state = DAVE_STATE_BURNING;
    dave->ticks_in_state = 0;
}

void dave_state_standing_enter(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
    int key_left, int key_right, int key_up) {
    dave->state = DAVE_STATE_STANDING;
    dave->ticks_in_state = 0;
    if (dave->mute == 0)
        dave->sfx->stop(dave->sfx);
}

void dave_state_jumping_enter(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
        int key_left, int key_right, int key_up) {
    if (dave->face_direction == DAVE_DIRECTION_FRONTL ||
        dave->face_direction == DAVE_DIRECTION_LEFT) {
        dave->face_direction = DAVE_DIRECTION_FRONTL;
    } else {
        dave->face_direction = DAVE_DIRECTION_FRONTR;
    }

    if (dave->mute == 0)
        dave->sfx->play(dave->sfx, TUNE_JUMPING);

    dave->state = DAVE_STATE_JUMPING;
    dave->walk_state = DAVE_STATE_STANDING;
    dave->jump_state = 0;
    dave->ticks_in_state = 0;
}

void dave_state_walking_enter(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
        int key_left, int key_right, int key_up) {
    dave->state = DAVE_STATE_WALKING;

    if (key_left) {
        if (dave_collision_left(dave, map)) {
            dave_state_standing_enter(dave, map, key_left, key_right, key_up);
            return;
        } else {
            dave->tile->x-=1;
            if (dave->jump_cooldown_count == 0 && !key_up) {
                if (dave->mute == 0)
                    dave->sfx->play(dave->sfx, TUNE_WALKING);
            }
            dave->face_direction = DAVE_DIRECTION_LEFT;
            dave->walk_state = DAVE_WALKING_STATE_COOLDOWN2_LEFT;
            dave->step_count++;
            if (!dave_collision_left(dave, map)) {
                dave->tile->x-=1;
            }
            return;
        }
    } else if (key_right) {
        if (dave_collision_right(dave, map)) {
            dave_state_standing_enter(dave, map, key_left, key_right, key_up);
            return;
        } else {
            if (dave->jump_cooldown_count == 0 && !key_up) {
                if (dave->mute == 0)
                    dave->sfx->play(dave->sfx, TUNE_WALKING);
            }
            dave->tile->x+=1;
            dave->face_direction = DAVE_DIRECTION_RIGHT;
            dave->walk_state = DAVE_WALKING_STATE_COOLDOWN2_RIGHT;
            dave->step_count++;
            if (!dave_collision_right(dave, map)) {
                dave->tile->x+=1;
            }
            return;
        }
    } else {
        dave_state_standing_enter(dave, map, key_left, key_right, key_up);
        return;
    }
}

void dave_state_jetpacking_routine(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
        int key_left, int key_right, int key_up, int key_down, int key_jetpack) {

    // Pressing jetpack key (alt) while in 'jetpack-state' will turn it off
    if (key_jetpack) {
        dave_state_standing_enter(dave, map, key_left, key_right, key_up);
        return;
    }

    if (dave->on_fire) {
        dave_state_burning_enter(dave, map, key_left, key_right, key_up);
        return;
    }

    if (key_left) {
        if (!dave_collision_left(dave, map)) {
            dave->tile->x-=1;
            dave->face_direction = DAVE_DIRECTION_LEFT;
        }
    } else if (key_right) {
        if (!dave_collision_right(dave, map)) {
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
    dave->jetpack_bars--;
    if (dave->jetpack_bars <= 0) {
        dave->jetpack_bars = 0;
        dave_state_standing_enter(dave, map, key_left, key_right, key_up);
    }

    if (dave->sfx->tune_idx == 0)
        dave->sfx->play(dave->sfx, TUNE_FLYING);
}

void dave_state_jetpacking_enter(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
        int key_left, int key_right, int key_up) {
    dave->state = DAVE_STATE_JETPACKING;
    dave->ticks_in_state = 0;
    dave->sfx->play(dave->sfx, TUNE_TOJETPACK);
}

static void dave_state_climbing_enter(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
        int key_left, int key_right, int key_up, int key_jetpack) {
    dave->state = DAVE_STATE_CLIMBING;
    if (key_up && key_right) {
        dave->climb_state = DAVE_CLIMBING_STATE_JUMP_RIGHT;
    } else if (key_up && key_left) {
        dave->climb_state = DAVE_CLIMBING_STATE_JUMP_LEFT;
    } else {
        dave->climb_state = DAVE_CLIMBING_STATE_READY;
    }
    dave->ticks_in_state = 0;
}

void dave_state_freefalling_enter(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
    int key_left, int key_right, int key_up) {
    dave->state = DAVE_STATE_FREEFALLING;

    if (dave->face_direction == DAVE_DIRECTION_RIGHT ||
        dave->face_direction == DAVE_DIRECTION_FRONTR) {
        dave->face_direction = DAVE_DIRECTION_FRONTR;

    } else if (dave->face_direction == DAVE_DIRECTION_LEFT ||
        dave->face_direction == DAVE_DIRECTION_FRONTL) {
        dave->face_direction = DAVE_DIRECTION_FRONTL;
    }
}

void dave_state_dead_routine(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT]) {
    dave->ticks_in_state++;
}

void dave_state_dead_enter(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT]) {
    dave->state = DAVE_STATE_DEAD;
}

void dave_state_burning_routine(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
        int key_left, int key_right, int key_up, int key_jetpack) {
    dave->ticks_in_state++;
    if (dave->ticks_in_state >= 200) {
        dave_state_dead_enter(dave, map);
    }
}

void dave_state_walking_routine(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
        int key_left, int key_right, int key_up, int key_jetpack) {
    if (dave->jump_cooldown_count > 0) {
        dave->jump_cooldown_count--;
    }

    if (dave->on_fire) {
        dave_state_burning_enter(dave, map, key_left, key_right, key_up);
        return;
    }

    if (key_jetpack && (dave->jetpack_bars > 0)) {
        dave_state_jetpacking_enter(dave, map, key_left, key_right, key_up);
        return;
    }

    if (!dave_on_ground(dave, map)) {
        dave_state_freefalling_enter(dave, map, key_left, key_right, key_up);
        dave->sfx->play(dave->sfx, TUNE_FALLING);
        return;
    }
    if (key_up) {
        if (dave->jump_cooldown_count <= 0) {
            dave_state_jumping_enter(dave, map, key_left, key_right, key_up);
            return;
        }
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

    } else {
        printf("BAD STATE! %d \n", dave->walk_state);
    }

}

void dave_state_jumping_routine(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
        int key_left, int key_right, int key_up, int key_jetpack) {

    int jump_velocity_table[95] = {
            -1,  0, -2,  0, -3,  0, -2,  0, -2,  0,
            -2,  0, -2,  0, -2,  0, -2,  0, -2,  0,
            -2,  0, -1,  0, -2,  0, -1,  0, -1,  0,
            -1,  0, -1,  0, -1,  0, -1,  0, -1,  0, //40 total up

             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
             0,  0,  0,  0,// 14 total hang

             0,  0,  1,  0,  1,  0,  1,  0,  1,  0, //40 total down
             1,  0,  1,  0,  1,  0,  2,  0,  2,  0,
             2,  0,  2,  0,  2,  0,  2,  0,  2,  0,
             2,  0,  2,  0,  2,  0,  3,  0,  2,  0};

    if (dave->on_fire) {
        dave_state_burning_enter(dave, map, key_left, key_right, key_up);
        return;
    }

    if (key_jetpack && (dave->jetpack_bars > 0)) {
        dave_state_jetpacking_enter(dave, map, key_left, key_right, key_up);
        return;
    }

    if (dave->jump_state >= 40 && dave_on_ground(dave, map)) {
        dave_state_standing_enter(dave, map, key_left, key_right, key_up);
        dave->jump_cooldown_count = 5;
        dave->step_count = 0;
        return;
    }

    if (dave->on_tree) {
        if ((key_up == 0) && (key_right || key_left)) {
            dave_state_climbing_enter(dave, map, key_left, key_right, key_up, key_jetpack);
        }

        if (key_up && (key_right || key_left) && dave->jump_state >= 3 && dave->jump_state <= 76) {
            dave->jump_cooldown_count = 1;
            dave_state_climbing_enter(dave, map, key_left, key_right, key_up, key_jetpack);

            dave->jump_state= 0;
            return;
        }
    }

    if (dave->jump_state == 94) {
        dave->tile->y = dave->tile->y + 2;

        if (dave_on_ground(dave, map)) {
            if (dave->mute == 0) {
                dave->sfx->stop(dave->sfx);
            }
            dave_state_walking_enter(dave, map, key_left, key_right, key_up);
            dave->jump_cooldown_count = 5;
            dave->step_count = 0;

        } else {
            dave->tile->y = dave->tile->y - 2;
            dave_state_freefalling_enter(dave, map, key_left, key_right, key_up);
        }

        return;
    }

    int deltay = jump_velocity_table[dave->jump_state];
    dave->jump_state++;

    // Handle Y axis when dave is on the way down
    while (deltay > 0) {
        dave->tile->y = dave->tile->y + 1;
        deltay--;

        if (dave_on_ground(dave, map)) {
            deltay = 0;
        }
    }

    // Handle Y axis when dave is on the way up
    while (deltay < 0) {
        dave->tile->y = dave->tile->y - 1;
        deltay++;

        // The original game predicts the collision with x axis, even though
        // it won't move it
        int is_collision_top = 0;
        if (dave_collision_top(dave, map)) {
            if (key_left) {
                dave->tile->x = dave->tile->x - 2;
            } else if (key_right) {
                dave->tile->x = dave->tile->x + 2;
            }
            is_collision_top = dave_collision_top(dave, map);
            if (key_left) {
                dave->tile->x = dave->tile->x + 2;
            } else if (key_right) {
                dave->tile->x = dave->tile->x - 2;
            }
        }

        if (is_collision_top) {
            dave->walk_state = DAVE_WALKING_STATE_STANDING;
            dave->jump_state = 94;
            return;
        }
    }

    // Temporary check
    if (dave->jump_state > 94) {
        printf("X94 \n");
        exit(0);
    }

    // Handle X axis with respect of walking state cooldown
    if (dave->walk_state == DAVE_STATE_STANDING) {
        if (key_left) {
            dave->face_direction = DAVE_DIRECTION_LEFT;

            if (!dave_collision_left(dave, map)) {
                dave->tile->x-=1;
                if (!dave_collision_left(dave, map)) {
                    dave->tile->x-=1;
                }
            }
            dave->walk_state = DAVE_WALKING_STATE_COOLDOWN2_LEFT;

        } else if (key_right) {
            dave->face_direction = DAVE_DIRECTION_RIGHT;

            if (!dave_collision_right(dave, map)) {
                dave->tile->x+=1;
                if (!dave_collision_right(dave, map)) {
                    dave->tile->x+=1;
                }
            }
            dave->walk_state = DAVE_WALKING_STATE_COOLDOWN2_RIGHT;
        }

        return;
    }

    if (dave->walk_state == DAVE_WALKING_STATE_COOLDOWN1_LEFT ||
            dave->walk_state == DAVE_WALKING_STATE_COOLDOWN2_LEFT ||
            dave->walk_state == DAVE_WALKING_STATE_COOLDOWN1_RIGHT ||
            dave->walk_state == DAVE_WALKING_STATE_COOLDOWN2_RIGHT) {
        dave->walk_state = DAVE_STATE_STANDING;
    }

}

static void dave_state_freefalling_routine(dave_t *dave,
        tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
    int key_left, int key_right, int key_up, int key_jetpack) {

    if (dave->on_fire) {
        dave_state_burning_enter(dave, map, key_left, key_right, key_up);
        return;
    }

    if (key_jetpack && (dave->jetpack_bars > 0)) {
        dave_state_jetpacking_enter(dave, map, key_left, key_right, key_up);
        return;
    }

    if (dave_on_ground(dave, map)) {
        dave_state_standing_enter(dave, map, key_left, key_right, key_up);
        dave->jump_cooldown_count = 5;
        dave->step_count = 0;
        return;
    }

    dave->tile->y = dave->tile->y + 1;
    if (key_left) {
        //dave->freefall_direction = DAVE_DIRECTION_LEFT;
        dave->face_direction = DAVE_DIRECTION_LEFT;
    } else if (key_right) {
        //dave->freefall_direction = DAVE_DIRECTION_RIGHT;
        dave->face_direction = DAVE_DIRECTION_RIGHT;
    }

    if (dave->face_direction == DAVE_DIRECTION_LEFT) {
        if (!dave_collision_left(dave, map)) {
            dave->tile->x = dave->tile->x - 1;
        }
    } else if (dave->face_direction == DAVE_DIRECTION_RIGHT) {
        if (!dave_collision_right(dave, map)) {
            dave->tile->x = dave->tile->x + 1;
        }
    }
}

static void dave_state_climbing_routine(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
        int key_left, int key_right, int key_up, int key_down, int key_jetpack) {

    if  (!dave->on_tree) {
        if (key_up) {
            dave_state_jumping_enter(dave, map, key_left, key_right, key_up);
            return;
        }
        dave_state_freefalling_enter(dave, map, key_left, key_right, key_up);
        return;
    }

    if (dave->on_fire) {
        dave_state_burning_enter(dave, map, key_left, key_right, key_up);
        return;
    }

    if (key_up && (key_left == 0) && (key_right == 0) && (key_down == 0)) {
        if (dave->climb_state == DAVE_CLIMBING_STATE_READY) {
            dave->tile->y = dave->tile->y - 2;
            dave->step_count++;
            dave->climb_state = DAVE_CLIMBING_STATE_COOLDOWN;
        } else {
            dave->climb_state = DAVE_CLIMBING_STATE_READY;
        }

    } else if (key_down && (key_left == 0) && (key_right == 0) && (key_up == 0)) {
        if (dave->climb_state == DAVE_CLIMBING_STATE_READY) {
            dave->tile->y = dave->tile->y + 1;
            if (!dave_on_ground(dave, map)) {
                dave->tile->y = dave->tile->y + 1;
            }
            dave->step_count++;
            dave->climb_state = DAVE_CLIMBING_STATE_COOLDOWN;
        } else {
            dave->climb_state = DAVE_CLIMBING_STATE_READY;
        }

        if (dave_on_ground(dave, map)) {
            dave_state_standing_enter(dave, map, key_left, key_right, key_up);
            return;
        }
    } else if (key_left && (key_up == 0) && (key_right == 0)) {
        if (dave->climb_state == DAVE_CLIMBING_STATE_READY) {
            dave->tile->x = dave->tile->x - 2;
            dave->step_count++;
            dave->climb_state = DAVE_CLIMBING_STATE_COOLDOWN;
        } else {
            dave->climb_state = DAVE_CLIMBING_STATE_READY;
        }

    } else if (key_right && (key_up == 0) && (key_left==0)) {
        if (dave->climb_state == DAVE_CLIMBING_STATE_READY) {
            dave->tile->x = dave->tile->x + 2;
            dave->step_count++;
            dave->climb_state = DAVE_CLIMBING_STATE_COOLDOWN;
        } else {
            dave->climb_state = DAVE_CLIMBING_STATE_READY;
        }

    } else if ((key_right || key_left) && key_up) {
        if (dave->jump_cooldown_count <= 0) {
            dave->tile->y-=1;
            dave_state_jumping_enter(dave, map, key_left, key_right, key_up);
            return;
        } else {
            dave->jump_cooldown_count--;
        }
    }

    dave->ticks_in_state++;
}

static void dave_state_standing_routine(dave_t *dave, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
        int key_left, int key_right, int key_up, int key_jetpack) {

    if (dave->on_fire) {
        dave_state_burning_enter(dave, map, key_left, key_right, key_up);
        return;
    }

    if (key_jetpack && (dave->jetpack_bars > 0)) {
        dave_state_jetpacking_enter(dave, map, key_left, key_right, key_up);
        return;
    }

    if (!dave_on_ground(dave, map)) {
        dave_state_freefalling_enter(dave, map, key_left, key_right, key_up);
        dave->sfx->play(dave->sfx, TUNE_FALLING);
        return;
    }

    if (dave->jump_cooldown_count > 0) {
        dave->jump_cooldown_count--;
    }

    if (key_up) {
        if (dave->on_tree) {
            dave_state_climbing_enter(dave, map, key_left, key_right, key_up, key_jetpack);
            return;
        }

        if (dave->jump_cooldown_count <= 0) {
            dave_state_jumping_enter(dave, map, key_left, key_right, key_up);
            return;
        }

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
    } else if (dave->state == DAVE_STATE_CLIMBING) {
        dave_state_climbing_routine(dave, map, key_left, key_right, key_up, key_down, key_jetpack);
    } else if (dave->state == DAVE_STATE_FREEFALLING) {
        dave_state_freefalling_routine(dave, map, key_left, key_right, key_up, key_jetpack);
    } else if (dave->state == DAVE_STATE_BURNING) {
        dave_state_burning_routine(dave, map, key_left, key_right, key_up, key_jetpack);
    } else if (dave->state == DAVE_STATE_JETPACKING) {
        dave_state_jetpacking_routine(dave, map, key_left, key_right, key_up, key_down, key_jetpack);
    } else if (dave->state == DAVE_STATE_DEAD) {
        dave_state_dead_routine(dave, map);
    }

    // clear collisions
    //dave->on_fire = 0;
    dave->on_tree = 0;
}

static int dave_is_dead(dave_t *dave) {
    if (dave->state == DAVE_STATE_DEAD) {
        return 1;
    }
    return 0;
}

/*
 * In this function we need to return the sprite (tile index) which need to be drawn for dave,
 * this would depend on state (jumping, walking, falling etc..) and the animation cycle in each state.
 * And for some states we consider the direction in which dave is standing/walking.
 */
static int dave_get_sprite(tile_t *tile) {
    int sprite = 0;
    int walk_mod;

    dave_t *dave = (dave_t *)tile->context;
    walk_mod = dave->step_count % 8;

    if (dave->state == DAVE_STATE_FREEFALLING) {
        if (dave->face_direction == DAVE_DIRECTION_LEFT) {
            sprite = SPRITE_IDX_DAVE_LEFT_HANDSFREE;
        } else if (dave->face_direction == DAVE_DIRECTION_RIGHT) {
            sprite = SPRITE_IDX_DAVE_RIGHT_HANDSFREE;
        } else if (dave->face_direction == DAVE_DIRECTION_FRONTR) {
            sprite = SPRITE_IDX_DAVE_JUMP_RIGHT;
        } else if (dave->face_direction == DAVE_DIRECTION_FRONTL) {
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
        if (dave->ticks_in_state > 100) {
            sprite = 0;
        } else if ((dave->ticks_in_state / 30) % 4 == 0) {
            sprite = SPRITE_IDX_EXPLOSION1;
        } else if ((dave->ticks_in_state / 30) % 4 == 1) {
            sprite = SPRITE_IDX_EXPLOSION2;
        } else if ((dave->ticks_in_state / 30) % 4 == 2) {
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
            } else {
                sprite = 0;
            }
        }
    } else if (dave->state == DAVE_STATE_CLIMBING) {
        if (dave->climb_state == DAVE_CLIMBING_STATE_READY || 
                dave->climb_state == DAVE_CLIMBING_STATE_COOLDOWN) {
            if (dave->step_count % 8 == 0 || dave->step_count % 8 == 1) {
                sprite = SPRITE_IDX_DAVE_CLIMB_HANDS_UP;
            } else if (dave->step_count % 8 == 2 || dave->step_count % 8 == 3 ||
                dave->step_count % 8 == 6 || dave->step_count % 8 == 7) {
                sprite = SPRITE_IDX_DAVE_CLIMB_HAND_RIGHT;
            } else if (dave->step_count % 8 == 4 || dave->step_count % 8 == 5) {
                sprite = SPRITE_IDX_DAVE_CLIMB_HAND_LEFT;
            } else {
                sprite = SPRITE_IDX_DAVE_FRONT;
            }
        } else if (dave->climb_state == DAVE_CLIMBING_STATE_JUMP_RIGHT) {

            sprite = SPRITE_IDX_DAVE_JUMP_RIGHT;
        } else if (dave->climb_state == DAVE_CLIMBING_STATE_JUMP_LEFT) {
            sprite = SPRITE_IDX_DAVE_JUMP_LEFT;
        } else {
            sprite = SPRITE_IDX_DAVE_FRONT;
        }
    } else {
        sprite = SPRITE_IDX_DAVE_FRONT;
    }

    return sprite;
}

dave_t* dave_create(soundfx_t *sfx, int x, int y) {
    dave_t *dave = malloc(sizeof(dave_t));
    dave->sfx = sfx;
    dave->mute = 0;
    dave->step_count = 0;
    dave->walk_state = DAVE_STATE_STANDING;
    dave->climb_state = 0;
    dave->face_direction = DAVE_DIRECTION_FRONTR;
    dave->jump_cooldown_count = 0;
    dave->has_trophy = 0;
    dave->has_gun = 0;
    dave->jetpack_bars = 0;
    dave->on_fire = 0;
    dave->on_tree = 0;
    dave->ticks_in_state = 0;
    dave->default_x = x;
    dave->default_y = y;
    dave->tick = &dave_tick;
    dave->is_dead = &dave_is_dead;

    dave->tile = malloc(sizeof(tile_t));
    dave->tile->x = x;
    dave->tile->y = y;
    dave->tile->width = 20;
    dave->tile->height = 16;
    dave->tile->collision_dx = 2;
    dave->tile->collision_dy = 2;
    dave->tile->collision_dw = -6;
    dave->tile->collision_dh = 0;

    dave->tile->mod = DAVE;
    dave->tile->tick = NULL;
    dave->tile->is_inside = NULL;
    dave->tile->context = dave;
    dave->tile->get_sprite = &dave_get_sprite;

    return dave;
}
