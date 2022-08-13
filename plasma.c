#include <stdio.h>
#include <stdlib.h>

#include "tile.h"
#include "plasma.h"


static int plasma_collision_right(plasma_t *plasma, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT]) {
    int idx = 0;
    for (idx = 0; idx < TILEMAP_WIDTH * TILEMAP_HEIGHT; idx++) {
        if (map[idx].sprites[0] != 0 && map[idx].mod == BRICK) {
            if(map[idx].is_inside(&map[idx], plasma->tile->x+20, plasma->tile->y+1)) {
                return 1;
            }
            if (map[idx].is_inside(&map[idx], plasma->tile->x+20, plasma->tile->y+1)) {
                return 1;
            }
        }
    }
    return 0;
}

static int plasma_collision_left(plasma_t *plasma, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT]) {
    int idx = 0;
    for (idx = 0; idx < TILEMAP_WIDTH * TILEMAP_HEIGHT; idx++) {
        if (map[idx].sprites[0] != 0 && map[idx].mod == BRICK) {
            if (map[idx].is_inside(&map[idx], plasma->tile->x - 2, plasma->tile->y + 1)) {
                return 1;
            }
            if (map[idx].is_inside(&map[idx], plasma->tile->x - 2, plasma->tile->y + 1)) {
                return 1;
            }
        }
    }
    return 0;
}

static void plasma_tick(plasma_t *plasma, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT],
        int deadzone_left, int deadzone_right) {
    if (plasma->state == PLASMA_STATE_DEAD) {
        return;
    }
    plasma->tile->x+=plasma->speed_x;

    if (plasma->tile->x >= deadzone_right) {
        plasma->state = PLASMA_STATE_DEAD;
    }

    if (plasma->tile->x <= deadzone_left) {
        plasma->state = PLASMA_STATE_DEAD;
    }

    if (plasma_collision_right(plasma, map) || plasma_collision_left(plasma, map)) {
        plasma->state = PLASMA_STATE_DEAD;
    }
}

static int plasma_is_dead(plasma_t *plasma) {
    return (plasma->state == PLASMA_STATE_DEAD) ? 1 : 0;
}

static int plasma_get_sprite(plasma_t *plasma) {
    if (plasma->state == PLASMA_STATE_DEAD) {
        return 0;
    }
    return plasma->tile->get_sprite(plasma->tile);
}

plasma_t* plasma_create_left(int x, int y) {
    plasma_t *plasma = malloc(sizeof(plasma_t));

    plasma->speed_x = -2;
    plasma->state = PLASMA_STATE_FLYING_LEFT;
    plasma->get_sprite = &plasma_get_sprite;
    plasma->tick = &plasma_tick;
    plasma->is_dead = &plasma_is_dead;

    plasma->tile = malloc(sizeof(tile_t));
    tile_create_plasma_left(plasma->tile, x, y, 20, 3);

    return plasma;
}

plasma_t* plasma_create_right(int x, int y) {
    plasma_t *plasma = malloc(sizeof(plasma_t));

    plasma->speed_x = 2;
    plasma->state = PLASMA_STATE_FLYING_RIGHT;
    plasma->get_sprite = &plasma_get_sprite;
    plasma->tick = &plasma_tick;
    plasma->is_dead = &plasma_is_dead;

    plasma->tile = malloc(sizeof(tile_t));
    tile_create_plasma_right(plasma->tile, x, y, 20, 3);

    return plasma;
}
