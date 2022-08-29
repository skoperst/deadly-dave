#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tile.h"
#include "monster.h"

/*
 * Spidy, monster of level3
 */
int spidy_path[] = {
     5,  3,  2,  4,  0,  5, -3,  4,    -4,  4, -5,  3, -7,  2, -6,  2,
    -6,  1, -7,  1, -8, -2, -5, -3,    -6, -3, -7, -2, -7, -3, -5, -2,
    -7, -3, -6, -3, -7, -3, -9, -2,   -10,  0, -9,  1, -7,  2, -9,  2,
    -8,  3, -8,  0, -9,  0, -9,  1,   -10,  2, -7,  0, -3, -1, -2, -4,
     1, -4,  3, -3,  3, -1,  3, -1,     6, -2,  8, -2,  8, -1, 11, -1,
    14, -1, 14, -2, 15,  0, 18,  0,    17,  0, 17,  1, 14,  1, 15,  1,
    10,  2,  6,  2,  6,  2
};

/*
 * Swirl, monster of level4
 */
int swirl_path[] = {
    -6, -4, -8, -2,-11,  0,-14,  0,    -10,  0,-11,  0,-11,  0,-10,  0,
    -9,  1, -5,  1, -3,  2, -2,  2,     -2,  4, -1,  5,  0,  6,  0,  6,
     1,  8,  5,  6, 11,  7,  9,  6,      8,  9,  2,  7,  6,  5,  4, 10,
    11,  6, 15,  2, 14, -2, 11, -3,      9, -8,  4, -7,  1, -7,  1, -8,
     0, -7,  0, -7,  0, -8, -1, -7,     -3, -8, -3, -5,  0, -5, -2, -5
};

/*
 * Sun, monster of level5
 */
int sun_path[] = {
    -2,  3,  1,  3, -2,  3, -4,  3,     -5, -2, -2, -3, -5,  0, -4,  2,
    -4,  3,  0,  0,  0,  1, -3,  5,     -2,  6, -1,  8,  2, 10,  8, 10,
    12,  8, 13,  6, 10,  6,  5,  3,      4, -1,  4, -3,  3, -4,  2, -6,
     1,-10, -3,-11, -7,-11,  0, -8,      3,-11, -4, -7, -6, -5, -7, -3,
    -5,  2, -2, 3
};

/*
 * Bones, monster of level6
 */
int bones_path[] = {
    6,  0, 10,  0,  9,  0,  9,  0,       9,  0, 10,  0,  9,  0,  9,  0,
    9,  0, 10,  0, 10,  0, 10,  0,      10,  0,  9,  0,  7,  0, -3,  0,
   -8,  0,-10,  0,-10,  0,-10,  0,     -10,  0,-10,  0, -8,  0, -10, 0,
   -9,  0,-10,  0, -9,  0, -8,  0,     -10,  0, -8,  0, -3,  0
};

/*
 * Ufo, monster of level7
 */
int ufo_path[] = {
    4,  0,  4,  1,  4,  2,  3,  2,       2,  4,  1,  5,  1,  7,  0,  9,
    0,  8,  0, 10, -2,  8, -5,  8,      -6,  7, -5,  9, -5,  7, -4,  1,
   -4,  0, -3, -1, -3, -3, -3, -6,      -3, -7, -1, -8, -2, -9,  2,-10,
    1,-10,  3, -9,  5, -9,  4, -6,       5, -5,  3, -3,  4, -2
};
/*
int guards_path[] = {
    -6, 1, -4, 2, -4, 3, -3, 4, -1, 3, -4, 3, -3, 2, -5, 2, -2, 3, 0, 4, 4, 2, 5, 2, 4, 1, 3, 3, 2, 5, 5, 3, 4, 2, 6, 1, 5, 0, 5, -1, 4, -2, 2, -3, 1, -3, 1, -3, 3, -3, 5, -1, 3, -1, 3, -2, 2, -3, 0, -3, -3, -2, -5, -2, -7, -1, -3, -2, -2, -3, -1, -3, -2, -3, -3, -3, -4, -2, -5, 0, 
};

int envy_path[] = {
    7, 1, 7, -1, 8, 2, 6, -2, 7, 2, 7, -2, 8, 2, 7, -2, -7, -1, -8, 3, -7, -2, -7, 2, -6, -2, -8, 2, -7, -2, -7, 0, 
};

int plate_path[] = {
    -5, 0, -6, 0, -13, 0, -21, 0, -30, 0, -29, 0, -17, 0, 3, 0, 5, 0, 4, 0, 15, 0, 8, 0, 20, 0, 19, 0, 17, 0, 12, 0, 18, 0
};
*/


monster_t* monster_create_ufo() {
    monster_t *monster = monster_create();
    monster->tile->width = 18;
    monster->tile->height = 8;

    memcpy(monster->route, ufo_path, sizeof(ufo_path));
    monster->route_sz = (sizeof(ufo_path)/sizeof(int));
    monster->fire_rate = 50;

    monster->tile->sprites[0] = SPRITE_IDX_MONSTER_UFO1;
    monster->tile->sprites[1] = SPRITE_IDX_MONSTER_UFO1;

    monster->tile->sprites[2] = SPRITE_IDX_MONSTER_UFO2;
    monster->tile->sprites[3] = SPRITE_IDX_MONSTER_UFO2;

    monster->tile->sprites[4] = SPRITE_IDX_MONSTER_UFO3;
    monster->tile->sprites[5] = SPRITE_IDX_MONSTER_UFO3;

    monster->tile->sprites[6] = SPRITE_IDX_MONSTER_UFO4;
    monster->tile->sprites[7] = SPRITE_IDX_MONSTER_UFO4;
    monster->tile->sprites[8] = 0;

    return monster;
}

monster_t* monster_create_bones() {
    monster_t *monster = monster_create();
    monster->tile->width = 24;
    monster->tile->height = 22;

    memcpy(monster->route, bones_path, sizeof(bones_path));
    monster->route_sz = (sizeof(bones_path)/sizeof(int));
    monster->fire_rate = 25;

    monster->tile->sprites[0] = SPRITE_IDX_MONSTER_BONES1;
    monster->tile->sprites[1] = SPRITE_IDX_MONSTER_BONES1;

    monster->tile->sprites[2] = SPRITE_IDX_MONSTER_BONES2;
    monster->tile->sprites[3] = SPRITE_IDX_MONSTER_BONES2;

    monster->tile->sprites[4] = SPRITE_IDX_MONSTER_BONES3;
    monster->tile->sprites[5] = SPRITE_IDX_MONSTER_BONES3;

    monster->tile->sprites[6] = SPRITE_IDX_MONSTER_BONES4;
    monster->tile->sprites[7] = SPRITE_IDX_MONSTER_BONES4;
    monster->tile->sprites[8] = 0;

    return monster;
}

monster_t* monster_create_swirl() {
    monster_t *monster = monster_create();

    memcpy(monster->route, swirl_path, sizeof(swirl_path));
    monster->route_sz = (sizeof(swirl_path)/sizeof(int));
    monster->fire_rate = 25;

    monster->tile->sprites[0] = SPRITE_IDX_MONSTER_SWIRL1;
    monster->tile->sprites[1] = SPRITE_IDX_MONSTER_SWIRL2;
    monster->tile->sprites[2] = SPRITE_IDX_MONSTER_SWIRL3;
    monster->tile->sprites[3] = SPRITE_IDX_MONSTER_SWIRL4;
    monster->tile->sprites[4] = SPRITE_IDX_MONSTER_SWIRL3;
    monster->tile->sprites[5] = SPRITE_IDX_MONSTER_SWIRL2;
    monster->tile->sprites[6] = 0;

    return monster;
}

monster_t* monster_create_sun() {
    monster_t *monster = monster_create();

    memcpy(monster->route, sun_path, sizeof(sun_path));
    monster->route_sz = (sizeof(sun_path)/sizeof(int));
    monster->fire_rate = 50;

    monster->tile->sprites[0] = SPRITE_IDX_MONSTER_SUN1;
    monster->tile->sprites[1] = SPRITE_IDX_MONSTER_SUN1;
    monster->tile->sprites[2] = SPRITE_IDX_MONSTER_SUN2;
    monster->tile->sprites[3] = SPRITE_IDX_MONSTER_SUN2;
    monster->tile->sprites[4] = SPRITE_IDX_MONSTER_SUN3;
    monster->tile->sprites[5] = SPRITE_IDX_MONSTER_SUN3;
    monster->tile->sprites[6] = SPRITE_IDX_MONSTER_SUN4;
    monster->tile->sprites[7] = SPRITE_IDX_MONSTER_SUN4;
    monster->tile->sprites[8] = 0;

    return monster;
}

monster_t* monster_create_spider() {
    monster_t *monster = monster_create();

    memcpy(monster->route, spidy_path, sizeof(spidy_path));
    monster->route_sz = (sizeof(spidy_path)/sizeof(int));
    monster->fire_rate = 25;

    monster->tile->sprites[0] = SPRITE_IDX_MONSTER_SPIDER1;
    monster->tile->sprites[1] = SPRITE_IDX_MONSTER_SPIDER2;
    monster->tile->sprites[2] = SPRITE_IDX_MONSTER_SPIDER3;
    monster->tile->sprites[3] = SPRITE_IDX_MONSTER_SPIDER4;
    monster->tile->sprites[4] = SPRITE_IDX_MONSTER_SPIDER3;
    monster->tile->sprites[5] = SPRITE_IDX_MONSTER_SPIDER2;
    monster->tile->sprites[6] = 0;

    return monster;
}

static void monster_state_dead_routine(monster_t *monster) {
    monster->ticks_in_state++;
}

static void monster_state_dead_enter(monster_t *monster) {
    monster->state = MONSTER_STATE_DEAD;
    monster->ticks_in_state = 0;
}

static void monster_state_burning_routine(monster_t *monster) {
    monster->ticks_in_state++;
    if (monster->ticks_in_state >= 100) {
        monster_state_dead_enter(monster);
    }
}

static void monster_state_burning_enter(monster_t *monster) {
    monster->state = MONSTER_STATE_BURNING;
    monster->cooldown = 0;
    monster->ticks_in_state = 0;
}

static void monster_state_active_routine(monster_t *monster, int dave_x) {
    monster->ticks_in_state++;
    if (monster->on_fire) {
        monster_state_burning_enter(monster);
        return;
    }

    if (monster->cooldown > 4) {
        monster->cooldown = 0;
    }

    if (monster->cooldown == 0) {
        monster->tile->x += monster->route[monster->route_idx];
        monster->tile->y += monster->route[monster->route_idx+1];
        monster->route_idx += 2;
        if (monster->route_idx >= monster->route_sz) {
            printf("ROUTE RESET ticks_in_state: %d \n", monster->ticks_in_state);
            monster->route_idx = 0;
        }
        monster->tile->sprite_idx++;
        if (monster->tile->sprites[monster->tile->sprite_idx] == 0) {
            monster->tile->sprite_idx = 0;
        }
    }

    monster->cooldown++;

    // Handle plasma creation
    printf("TICKS BEFORE SHOOT: %d \n", monster->ticks_before_shoot);
    if (monster->plasma == NULL) {
        if (monster->ticks_before_shoot == 0) {
            if (dave_x > monster->tile->x) {
                monster->plasma = plasma_create_right(monster->tile->x - 8, monster->tile->y + 8);
            } else {
                monster->plasma = plasma_create_left(monster->tile->x - 21, monster->tile->y + 8);
            }
        } else {
            monster->ticks_before_shoot--;
        }
    } else {
        monster->ticks_before_shoot = monster->fire_rate;
    }
}

static void monster_tick(monster_t *monster, int dave_x) {


    if (monster->state == MONSTER_STATE_ACTIVE) {
        monster_state_active_routine(monster, dave_x);
    } else if (monster->state == MONSTER_STATE_BURNING) {
        monster_state_burning_routine(monster);
    } else if (monster->state == MONSTER_STATE_DEAD) {
        monster_state_dead_routine(monster);
    }

}

static int monster_get_sprite(tile_t *tile) {
    int sprite = 0;

    monster_t *monster = (monster_t *)tile->context;
    if (monster->state == MONSTER_STATE_ACTIVE) {
        sprite = tile->sprites[tile->sprite_idx];
    } else if (monster->state == MONSTER_STATE_BURNING) {
        if (monster->ticks_in_state > 100) {
            sprite = 0;
        } else if ((monster->ticks_in_state / 30) % 4 == 0) {
            sprite = SPRITE_IDX_EXPLOSION3;
        } else if ((monster->ticks_in_state / 30) % 4 == 1) {
            sprite = SPRITE_IDX_EXPLOSION4;
        } else if ((monster->ticks_in_state / 30) % 4 == 2) {
            sprite = SPRITE_IDX_EXPLOSION1;
        } else {
            sprite = SPRITE_IDX_EXPLOSION2;
        }
    } else if (monster->state == MONSTER_STATE_DEAD) {
        sprite = 0;
    }

    return sprite;
}

static int monster_is_alive(monster_t *monster) {
    if (monster->state == MONSTER_STATE_ACTIVE) {
        return 1;
    }
    return 0;
}

monster_t* monster_create() {
    monster_t *monster = malloc(sizeof(monster_t));

    monster->state = MONSTER_STATE_ACTIVE;
    monster->route_idx = 0;
    monster->cooldown = 0;
    monster->ticks_in_state = 0;
    monster->on_fire = 0;
    monster->ticks_before_shoot = 0;
    monster->tick = &monster_tick;
    monster->is_alive = &monster_is_alive;

    monster->tile = malloc(sizeof(tile_t));
    monster->tile->x = 0;
    monster->tile->y = 0;
    monster->tile->width = 24;
    monster->tile->height = 21;
    monster->tile->collision_dx = 0;
    monster->tile->collision_dy = 0;
    monster->tile->collision_dw = 0;
    monster->tile->collision_dh = 0;

    monster->tile->mod = MONSTER;
    monster->tile->sprites[0] = 0;
    monster->tile->sprite_idx = 0;
    monster->tile->context = monster;
    monster->tile->get_sprite = &monster_get_sprite;

    monster->plasma = NULL;

    return monster;
}
