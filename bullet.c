#include <stdio.h>
#include <stdlib.h>

#include "tile.h"
#include "bullet.h"


static int bullet_collision_right(bullet_t *bullet, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT]) {
    int idx = 0;
    for (idx = 0; idx < TILEMAP_WIDTH * TILEMAP_HEIGHT; idx++) {
        if (map[idx].sprites[0] != 0 && map[idx].mod == BRICK) {
            if(map[idx].is_inside(&map[idx], bullet->tile->x+10, bullet->tile->y+1)) {
                return 1;
            }
            if (map[idx].is_inside(&map[idx], bullet->tile->x+10, bullet->tile->y+1)) {
                return 1;
            }
        }
    }
    return 0;
}

static int bullet_collision_left(bullet_t *bullet, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT]) {
    int idx = 0;
    for (idx = 0; idx < TILEMAP_WIDTH * TILEMAP_HEIGHT; idx++) {
        if (map[idx].sprites[0] != 0 && map[idx].mod == BRICK) {
            if (map[idx].is_inside(&map[idx], bullet->tile->x - 2, bullet->tile->y + 1)) {
                return 1;
            }
            if (map[idx].is_inside(&map[idx], bullet->tile->x - 2, bullet->tile->y + 1)) {
                return 1;
            }
        }
    }
    return 0;
}




static void bullet_tick(bullet_t *bullet, tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT]) {
    printf("bullet-tick \n");
    if (bullet->state == BULLET_STATE_DEAD) {
        return;
    }
    bullet->tile->x+=bullet->speed_x;
    if (bullet->tile->x > 320) {
        bullet->state = BULLET_STATE_DEAD;
        return;
    }

    bullet->steps++;
    if (bullet->steps > 120) {
        bullet->state = BULLET_STATE_DEAD;
    }

    if (bullet_collision_right(bullet, map) || bullet_collision_left(bullet, map)) {
        bullet->state = BULLET_STATE_DEAD;
    }
}

static int bullet_is_dead(bullet_t *bullet) {
    return (bullet->state == BULLET_STATE_DEAD) ? 1 : 0;
}

int bullet_get_sprite(tile_t *tile) {
    bullet_t *bullet = (bullet_t *)tile->context;

    if (bullet->state == BULLET_STATE_FLYING_LEFT) {
        return SPRITE_IDX_BULLET_LEFT;
    } else {
        return SPRITE_IDX_BULLET_RIGHT;
    }
}

/**
 * Create a bullet that flys left
 */
bullet_t* bullet_create_left(int x, int y) {
    bullet_t *bullet = malloc(sizeof(bullet_t));

    // According to measurements a bullet moves 2 pixels per tick
    bullet->speed_x = -2;
    bullet->state = BULLET_STATE_FLYING_LEFT;
    bullet->steps = 0;

    bullet->tick = &bullet_tick;
    bullet->is_dead = &bullet_is_dead;

    bullet->tile = malloc(sizeof(tile_t));
    bullet->tile->x = x;
    bullet->tile->y = y;
    bullet->tile->width = 0;
    bullet->tile->height = 0;
    bullet->tile->tick = NULL;
    bullet->tile->is_inside = NULL;
    bullet->tile->context = bullet;
    bullet->tile->get_sprite = &bullet_get_sprite;

    return bullet;
}

/**
 * Create a bullet that flys right
 */
bullet_t* bullet_create_right(int x, int y) {
    bullet_t *bullet = malloc(sizeof(bullet_t));

    // According to measurements a bullet moves 2 pixels per tick
    bullet->speed_x = 2;
    bullet->state = BULLET_STATE_FLYING_RIGHT;
    bullet->steps = 0;

    bullet->tick = &bullet_tick;
    bullet->is_dead = &bullet_is_dead;

    bullet->tile = malloc(sizeof(tile_t));
    bullet->tile->x = x;
    bullet->tile->y = y;
    bullet->tile->width = 0;
    bullet->tile->height = 0;
    bullet->tile->tick = NULL;
    bullet->tile->is_inside = NULL;
    bullet->tile->context = bullet;
    bullet->tile->get_sprite = &bullet_get_sprite;

    return bullet;
}
