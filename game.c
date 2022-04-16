#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <execinfo.h>
#include <math.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "game.h"
#include "soundfx.h"

#define DEBUG_INPUT 1

const int G_STATE_NONE             = 0;
const int G_STATE_INTRO            = 1;
const int G_STATE_INTRO_POPUP      = 2;
const int G_STATE_LEVEL_START      = 3;
const int G_STATE_LEVEL            = 4;
const int G_STATE_LEVEL_POPUP      = 5;
const int G_STATE_WARP_RIGHT       = 6;
const int G_STATE_POPUP            = 7;
const int G_STATE_GAMEOVER         = 8;
const int G_STATE_QUIT_NOW         = 9;

SDL_Window* g_window;
SDL_Renderer* g_renderer;
SDL_Texture *g_texture;
uint32_t        *g_pixels;
SDL_Texture* g_render_texture;
assets_t* g_assets;
soundfx_t *g_soundfx;


void render_tile_idx(int tile_idx, int x, int y) {
    SDL_Surface *surface = g_assets->tiles[tile_idx];

    int blend = 0;
    if (tile_idx == SPRITE_IDX_BULLET_RIGHT || tile_idx == SPRITE_IDX_BULLET_LEFT ||
            tile_idx == SPRITE_IDX_MONSTER_SUN1 || tile_idx == SPRITE_IDX_MONSTER_SUN2 ||
            tile_idx == SPRITE_IDX_MONSTER_SUN3 || tile_idx == SPRITE_IDX_MONSTER_SUN4 ||
            tile_idx == SPRITE_IDX_MONSTER_SPIDER1 || tile_idx == SPRITE_IDX_MONSTER_SPIDER2 ||
            tile_idx == SPRITE_IDX_MONSTER_SPIDER3 || tile_idx == SPRITE_IDX_MONSTER_SPIDER4) {
        blend = 1;
    }

    int column_idx, line_idx;
    for (line_idx = 0; line_idx < surface->h; line_idx++) {
        for (column_idx = 0; column_idx < surface->w; column_idx++) {
            uint32_t pixel = ((uint32_t*)surface->pixels)[line_idx * surface->w + column_idx];

            if ( (pixel & 0x000000FF) == 0) { // is a pixel totally transperant dont draw it
            } else if (line_idx + y >= 200) {
            } else if (column_idx + x >= 320) {
            } else if (column_idx + x < 0) {
            } else {
                if ( (line_idx + y) >= 0) {
                    if (blend) {
                        uint32_t oldpixel = g_pixels[(line_idx + y) * 320 + (column_idx + x)];

                        if (oldpixel != 0x000000FF) {
                            pixel = (pixel ^ oldpixel);
                        }
                    }
                    g_pixels[(line_idx + y) * 320 + (column_idx + x)] = pixel;
                }
            }
        }
    }
}

void draw_tile_offset(tile_t *tile, struct game_assets *assets, int x_offset) {
    render_tile_idx(tile->get_sprite(tile), tile->x - (x_offset * 16), tile->y);
}

void draw_tile(tile_t *tile, struct game_assets *assets) {
    draw_tile_offset(tile, assets, 0);
}

void draw_char(char c, int x, int y, SDL_Renderer *renderer) {
    int tile_idx;
    int white_letters_idx = 500;
    //int black_letters_idx = 600;
    int letters[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 
        'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', \
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ' ', ',', '.', '(', \
        ')', '!', '?'};

    int idx;
    for (idx = 0; idx < sizeof(letters) / sizeof(int); idx++) {
        if ((int)c == letters[idx]) {
            tile_idx = white_letters_idx + idx;
            render_tile_idx(tile_idx, x, y);
            return;
        }
    }
}
void draw_text_line(const char *line, int x, int y, SDL_Renderer *renderer) {
    int i = 0;
    for (i = 0; i < strlen(line); i++) {
        draw_char(line[i], x + (i*8), y, renderer);
    }
}

void draw_char_black(char c, int x, int y, SDL_Renderer *renderer) {
    int tile_idx;
    int black_letters_idx = 600;
    int letters[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 
        'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', \
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ' ', ',', '.', '(', \
        ')', '!', '?'};

    int idx;
    for (idx = 0; idx < sizeof(letters) / sizeof(int); idx++) {
        if ((int)c == letters[idx]) {
            tile_idx = black_letters_idx + idx;
            render_tile_idx(tile_idx, x, y);
            return;
        }
    }
}

void draw_text_line_black(const char *line, int x, int y, SDL_Renderer *renderer) {
    int i = 0;
    for (i = 0; i < strlen(line); i++) {
        draw_char_black(line[i], x + (i*8), y, renderer);
    }
}

void draw_popup_box(int x, int y, int rows, int columns)
{
    tile_t popup_box[40][40];
    int cur_sprite;
    int col, row;
    for (row = 0; row < rows; row++) {
        for (col = 0; col < columns; col++) {
            if (row == 0) {
                if (col == 0) {
                    cur_sprite = SPRITE_IDX_POPUP_BOX_T1;
                } else if (col == (columns - 1)) {
                    cur_sprite = SPRITE_IDX_POPUP_BOX_T3;
                } else {
                    cur_sprite = SPRITE_IDX_POPUP_BOX_T2;
                }
            } else if (row == (rows - 1)) {
                if (col == 0) {
                    cur_sprite = SPRITE_IDX_POPUP_BOX_B1;
                } else if (col == (columns-1)) {
                    cur_sprite = SPRITE_IDX_POPUP_BOX_B3;
                } else {
                    cur_sprite = SPRITE_IDX_POPUP_BOX_B2;
                }
            } else {
                if (col == 0) {
                    cur_sprite = SPRITE_IDX_POPUP_BOX_M1;
                } else if (col == (columns - 1)) {
                    cur_sprite = SPRITE_IDX_POPUP_BOX_M3;
                } else { 
                    cur_sprite = SPRITE_IDX_POPUP_BOX_M2;
                }
            }
            tile_create_block(&popup_box[row][col], cur_sprite, x + (col * 8), y + (row * 8), 8, 8);
        }
    }

    // Actual draw
    for (row = 0; row < rows; row++) {
        for (col = 0; col < columns; col++) {
            draw_tile(&popup_box[row][col], g_assets);
        }
    }
}

void draw_map_offset(tile_t *map, int offset) {
    int i;

    for (i = 0; i < (TILEMAP_SCENE_WIDTH * TILEMAP_SCENE_HEIGHT); i++) {
        if (map[i + (offset * 12)].sprites[0] != 0) {
            draw_tile_offset(&map[i + (offset*12)], g_assets, offset);
        }
    }
}

void draw_bullet_offset(bullet_t *bullet, struct game_assets *assets, int x_offset) {
    draw_tile_offset(bullet->tile, assets, x_offset);
}

void draw_dave_offset(dave_t *dave, struct game_assets *assets, int x_offset) {
    if (dave->tile->get_sprite(dave->tile) != 0) {
        draw_tile_offset(dave->tile, assets, x_offset);
    }
}

void draw_monster_offset(monster_t *monster, struct game_assets *assets, int x_offset) {

/*    printf("drawing monster x:%d, y:%d \n", monster->tile->x, monster->tile->y);
    printf("drawing monster with offsset: x:%d, y:%d \n", 
            (monster->tile->x - (x_offset * 16)), monster->tile->y);*/
    if (monster->tile->get_sprite(monster->tile) != 0) {
        render_tile_idx(monster->tile->get_sprite(monster->tile),
            monster->tile->x - (x_offset * 16), monster->tile->y);
    }
}

void draw_x_levels_to_go(int x) {
    char good_work[128];
    snprintf(good_work, sizeof(good_work), "GOOD WORK! ONLY %d MORE TO GO!", x);
    draw_text_line(good_work, 50, 58, g_renderer);
}

void draw_jetpack(int bars)
{
    int i = 0;
    if (bars < 0) {
        bars = 0;
    } else if (bars > 600) {
        bars = 600;
    }

    render_tile_idx(SPRITE_IDX_JETPACK_LABEL, 0, 170);
    render_tile_idx(SPRITE_IDX_JETPACK_BAR_FRAME, 72, 170);
    for (i = 0; i< (bars/10); i++) {
        render_tile_idx(SPRITE_IDX_JETPACK_BAR, 76 + i*2, 174);
    }
}

void draw_level(int level) {
    render_tile_idx(136, 104, 0); // level:
    render_tile_idx(148, 176, 0); //0
    render_tile_idx(148 + level + 1, 184, 0); //0
}

void draw_lives(int lives) {
    int start_x = 256;
    int idx = 0;

    render_tile_idx(135, 192, 0);

    for (idx = 0; (idx < (lives - 1)) && (idx < 4); idx++) {
        render_tile_idx(143, start_x + (16 * idx), 0);
    }
}

/**
 * Draws score on top left corner. Score is limited to 5 digits.
 */
void draw_score(int score) {
    int i;
    render_tile_idx(137, 0, 0);
    for (i = 0; i < 5; i++) {
        int mod = score % 10;
        score = score / 10;
        render_tile_idx(148 + mod, 96 - (8 * i), 0);
    }
}

void init_assets(struct game_assets *assets, SDL_Renderer *renderer) {
    int i;
    char fname[50];
    char file_num[4];
    SDL_Surface *surface;

    for (i = 0; i < 1000; i++) {
        assets->tiles[i] = NULL;
    }

    for (i=0; i<1000; i++) {
        fname[0]='\0';
        strcat(fname, "res/tiles/tile");
        sprintf(&file_num[0],"%u",i);
        strcat(fname, file_num);
        strcat(fname, ".bmp");

        if (access(fname, F_OK) == 0)
        {
            surface = SDL_LoadBMP(fname);
            assets->tiles[i] = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);

            // Monster tiles should use black transparency
            if ((i >= 89 && i <= 120 ) || (i >= 129 && i <= 132 )) {
                SDL_SetColorKey(surface, 1, SDL_MapRGB(surface->format, 0x00, 0x00, 0x00));
            }
            assets->graphics_tiles[i] = SDL_CreateTextureFromSurface(renderer, surface);
        }
    }
}

/* Set game and monster properties to default values */
void init_game(game_context_t *game) {
    game->quit = 0;
    game->tick = 0;
    game->lives = 4;
    game->score = 0;
    game->scroll_x = 0;
    game->dave_right = 0;
    game->dave_left = 0;
    game->dave_jump = 0;
    game->dave_fire = 0;
    game->dave_down = 0;
    game->dave_up = 0;
    game->dave_climb = 0;
    game->dave_jetpack = 0;
    game->jetpack_delay = 0;
    game->last_dir = 0;
    game->jump_timer = 0;
    game->on_ground = 1;
    game->check_pickup_x = 0;
    game->check_pickup_y = 0;
    game->check_door = 0;

    game->scroll_offset = 0;
    game->scroll_remaining = 0;

    game->bullet = NULL;
    game->level = 0;
}

void get_keys(keys_state_t* state) {
    SDL_Event event;

    const uint8_t *keystate = SDL_GetKeyboardState(NULL);
    state->right      = (keystate[SDL_SCANCODE_RIGHT]  != 0) ? 1 : 0;
    state->left       = (keystate[SDL_SCANCODE_LEFT]   != 0) ? 1 : 0;
    state->jump       = (keystate[SDL_SCANCODE_UP]     != 0) ? 1 : 0;
    state->down       = (keystate[SDL_SCANCODE_DOWN]   != 0) ? 1 : 0;
    state->escape     = (keystate[SDL_SCANCODE_ESCAPE] != 0) ? 1 : 0;
    state->fire       = (keystate[SDL_SCANCODE_LCTRL]  != 0) ? 1 : 0;
//    state->enter     = (keystate[SDL_SCANCODE_RETURN] != 0) ? 1 : 0;
    state->space      = (keystate[SDL_SCANCODE_SPACE]  != 0) ? 1 : 0;
    state->key_y      = (keystate[SDL_SCANCODE_Y] != 0) ? 1 : 0;
    state->key_n      = (keystate[SDL_SCANCODE_N] != 0) ? 1 : 0;

    state->jetpack = 0;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_USEREVENT) {
            printf("user event \n");
        } else if (event.type == SDL_KEYDOWN) {
            int is_repeat = event.key.repeat;
            int modifier = event.key.keysym.mod;
            int scancode = event.key.keysym.scancode;

            printf("mod: %d, scan: %d, repeat: %d \n", modifier, scancode, is_repeat);
            if (event.key.keysym.scancode == SDL_SCANCODE_LALT) {
                state->jetpack = 1;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_RETURN && is_repeat == 0) {
                state->enter = 1;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_RIGHTBRACKET && is_repeat == 0) {
                state->bracer = 1;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_LEFTBRACKET && is_repeat == 0) {
                state->bracel = 1;
            }
        } else if (event.type == SDL_QUIT) {
            state->quit = 1;
        }
    }
}

int start_intro() {
    int32_t intro_should_finish = 0;
    uint32_t timer_begin;
    uint32_t timer_end;
    uint32_t delay;
    game_context_t *game;
    int32_t result = 0;
    int stride;
    int idx;

    keys_state_t key_state = {0, 0, 0, 0, 0, 0, 0, 0};
    /* Clear screen */
    SDL_SetRenderDrawColor(g_renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(g_renderer);

    get_keys(&key_state);
    get_keys(&key_state);
    get_keys(&key_state);

    memset(&key_state, 0x00, sizeof(keys_state_t));
    printf("start_intro E \n");
    /* Initialize game state */
    game = malloc(sizeof(game_context_t));
    init_game(game);

    tile_t block[41];

    tile_create_intro_banner(&block[0], 103, 0);
    tile_create_block(&block[1], SPRITE_IDX_DIRT, 88, 64, TILE_SIZE, TILE_SIZE);
    tile_create_block(&block[2], SPRITE_IDX_DIRT, 120, 64, TILE_SIZE, TILE_SIZE);
    tile_create_block(&block[3], SPRITE_IDX_DIRT, 136, 64, TILE_SIZE, TILE_SIZE);
    tile_create_block(&block[4], SPRITE_IDX_DIRT, 152, 64, TILE_SIZE, TILE_SIZE);
    tile_create_block(&block[5], SPRITE_IDX_DIRT, 168, 64, TILE_SIZE, TILE_SIZE);
    tile_create_block(&block[6], SPRITE_IDX_DIRT, 200, 64, TILE_SIZE, TILE_SIZE);
    tile_create_block(&block[7], SPRITE_IDX_DIRT, 232, 64, TILE_SIZE, TILE_SIZE);

    tile_create_block(&block[8], SPRITE_IDX_DIRT, 88, 80, TILE_SIZE, TILE_SIZE);
    tile_create_block(&block[9], SPRITE_IDX_DIRT, 120, 80, TILE_SIZE, TILE_SIZE);
    tile_create_block(&block[10], SPRITE_IDX_CROWN, 136, 80, TILE_SIZE, TILE_SIZE);
    tile_create_block(&block[11], SPRITE_IDX_DIRT, 232, 80, TILE_SIZE, TILE_SIZE);

    tile_create_block(&block[12], SPRITE_IDX_DIRT, 88, 96, TILE_SIZE, TILE_SIZE);
    tile_create_block(&block[13], SPRITE_IDX_DIRT, 120, 96, TILE_SIZE, TILE_SIZE);
    tile_create_block(&block[14], SPRITE_IDX_DIRT, 168, 96, TILE_SIZE, TILE_SIZE);
    tile_create_block(&block[15], SPRITE_IDX_DIRT, 184, 96, TILE_SIZE, TILE_SIZE);
    tile_create_block(&block[16], SPRITE_IDX_DIRT, 200, 96, TILE_SIZE, TILE_SIZE);
    tile_create_block(&block[17], SPRITE_IDX_DIRT, 232, 96, TILE_SIZE, TILE_SIZE);

    tile_create_block(&block[18], SPRITE_IDX_DIRT, 88, 112, TILE_SIZE, TILE_SIZE);
    tile_create_block(&block[19], SPRITE_IDX_DIRT, 120, 112, TILE_SIZE, TILE_SIZE);
    tile_create_block(&block[20], SPRITE_IDX_CROWN, 216, 112, TILE_SIZE, TILE_SIZE);
    tile_create_block(&block[21], SPRITE_IDX_DIRT, 232, 112, TILE_SIZE, TILE_SIZE);

    tile_create_block(&block[22], SPRITE_IDX_DIRT, 88, 128, TILE_SIZE, TILE_SIZE);
    tile_create_block(&block[23], SPRITE_IDX_DIRT, 120, 128, TILE_SIZE, TILE_SIZE);
    tile_create_intro_fire(&block[24], 136, 128);
    tile_create_block(&block[25], SPRITE_IDX_DIRT, 152, 128, TILE_SIZE, TILE_SIZE);
    tile_create_block(&block[26], SPRITE_IDX_DIRT, 168, 128, TILE_SIZE, TILE_SIZE);
    tile_create_block(&block[27], SPRITE_IDX_DIRT, 184, 128, TILE_SIZE, TILE_SIZE);
    tile_create_block(&block[28], SPRITE_IDX_DIRT, 232, 128, TILE_SIZE, TILE_SIZE);

    tile_create_block(&block[29], SPRITE_IDX_DIRT, 88, 144, TILE_SIZE, TILE_SIZE);
    tile_create_block(&block[30], SPRITE_IDX_DIRT, 232, 144, TILE_SIZE, TILE_SIZE);

    tile_create_block(&block[31], SPRITE_IDX_DIRT, 88, 160, TILE_SIZE, TILE_SIZE);
    tile_create_block(&block[32], SPRITE_IDX_DIRT, 104, 160, TILE_SIZE, TILE_SIZE);
    tile_create_block(&block[33], SPRITE_IDX_DIRT, 120, 160, TILE_SIZE, TILE_SIZE);
    tile_create_block(&block[34], SPRITE_IDX_DIRT, 136, 160, TILE_SIZE, TILE_SIZE);
    tile_create_block(&block[35], SPRITE_IDX_DIRT, 152, 160, TILE_SIZE, TILE_SIZE);
    tile_create_block(&block[36], SPRITE_IDX_DIRT, 168, 160, TILE_SIZE, TILE_SIZE);
    tile_create_block(&block[37], SPRITE_IDX_DIRT, 184, 160, TILE_SIZE, TILE_SIZE);
    tile_create_block(&block[38], SPRITE_IDX_DIRT, 200, 160, TILE_SIZE, TILE_SIZE);
    tile_create_intro_fire(&block[39], 216, 160);
    tile_create_block(&block[40], SPRITE_IDX_DIRT, 232, 160, TILE_SIZE, TILE_SIZE);

    while (!intro_should_finish) {
        timer_begin = SDL_GetTicks();

        get_keys(&key_state);

        // All next drawings will be done into the texture (as oppose to screen)
        if (key_state.escape) {
            printf("SDL_QUIT \n");
            exit(0);
        }
        if (key_state.enter || key_state.space) {

            result = 0;
            intro_should_finish = 1;
        }
        //if (key_state
        /*while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                printf("SDL_QUIT \n");
                exit(0);
            }
            const uint8_t *keystate = SDL_GetKeyboardState(NULL);
            if (keystate[SDL_SCANCODE_ESCAPE]) {
                printf("ESC \n");
                //show_quit_popup();
            }
            if ( keystate[SDL_SCANCODE_RIGHT] ||
                keystate[SDL_SCANCODE_LEFT] ||
                keystate[SDL_SCANCODE_UP] ||
                keystate[SDL_SCANCODE_DOWN] ||
                keystate[SDL_SCANCODE_RETURN] ||
                keystate[SDL_SCANCODE_SPACE]) {
                result = 0;
                intro_should_finish = 1;
            }
        }*/

        SDL_SetRenderDrawColor(g_renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(g_renderer);
        SDL_LockTexture(g_texture, NULL, (void*)&g_pixels, &stride);

        for (idx = 0; idx < 320 * 200; idx++) {
            g_pixels[idx] = 0x000000FF;
        }
        // Draw all tiles
        for (idx = 0; idx < 41; idx++) {
            draw_tile(&block[idx], g_assets);
            block[idx].tick(&block[idx]);
        }

        draw_text_line("BY JOHN ROMERO", 110, 50, g_renderer);
        draw_text_line("(C) 1990 SOFTDISK, INC.", 79, 57, g_renderer);
        draw_text_line("                         ",72, 166, g_renderer);
        draw_text_line("PRESS THE F1 KEY FOR HELP", 72, 168, g_renderer);
        draw_text_line("                         ",72, 174, g_renderer);

        SDL_UnlockTexture(g_texture);
        SDL_RenderCopy(g_renderer, g_texture, NULL,NULL);
        SDL_RenderPresent(g_renderer);

        timer_end = SDL_GetTicks();
        delay = 14 - (timer_end-timer_begin);
        delay = delay > 14 ? 0 : delay;
        SDL_Delay(delay);
    }

    // Clean up and quit
    free(game);

    return result;
}

void clear_monsters(game_context_t *game) {
    int i = 0;
    for (i = 0; i < 5; i++) {
        game->monsters[i] = NULL;
    }
}

void clear_map(tile_t *map) {
    int i = 0;

    for (i = 0; i < TILEMAP_WIDTH * TILEMAP_HEIGHT; i++) {
        map[i].sprites[0] = 0;
        map[i].sprites[1] = 0;
        map[i].mod = 0;
        map[i].x = 0;
        map[i].y = 0;
    }
}

int game_popup_quit_routine(game_context_t *game, tile_t *map,
    tile_t *flashing_cursor, keys_state_t *keys) {
    if (keys->quit) {
        printf("should quit! \n");
        return G_STATE_QUIT_NOW;
    }

    if (keys->key_y) {
        return G_STATE_QUIT_NOW;
    }

    if (keys->key_n) {
        g_soundfx->resume(g_soundfx);
        return G_STATE_LEVEL;
    }

    draw_popup_box(88, 80, 5, 21);
    draw_text_line_black("QUIT? (Y OR N):", 104, 98, g_renderer);
    draw_tile(flashing_cursor, g_assets);
    flashing_cursor->tick(flashing_cursor);

    return G_STATE_LEVEL_POPUP;
}


// When dave nears screen edge and there is more tiles in that edge,
// we will scroll the screen to make new screen visible under the assumption
// usually the player moving to one direction.
//
// offset = 0                             offset = 0 + delta
// +-------------------+                 +--------------------+
// |        1          .                 .           2        |
// |                   .                 .                    |
// |              DAVE .      ---->      . DAVE               |
// |                   .                 .                    |
// +-------------------+                 +--------------------+
int game_adjust_scroll_to_dave(game_context_t *game, dave_t *dave) {
    /* Here we still have scrolling to do so we just scroll the screen a bit */
    if (game->scroll_remaining != 0) {
        if (game->scroll_remaining > 0) {
            game->scroll_remaining--;
            if ((game->scroll_remaining % 1) == 0) {
                game->scroll_offset++;
            }
        } else if (game->scroll_remaining < 0) {
            game->scroll_remaining++;
            if ((game->scroll_remaining % 1) == 0) {
                game->scroll_offset--;
            }
        }
        return 1;
    }
    /*
     * If we got here we have no previous scrolling in progress and we need to caluclate 
     * and decide if any scrolling is needed. 
     */
    else {
        int delta = (game->dave->tile->x - (game->scroll_offset * 16));
        if (delta > 320 - (16 + 16 + 8) && game->scroll_offset < 80) {
            if ((80 -  game->scroll_offset) < 15) {
                game->scroll_remaining = (80 - game->scroll_offset);
            } else {
                game->scroll_remaining = 15;
            }
            return 1;

        } else if (delta < (16 + 14) && game->scroll_offset > 0) {
            if (game->scroll_offset < 15) {
                game->scroll_remaining = (0 - game->scroll_offset);
            } else {
                game->scroll_remaining = -15;
            }
            return 1;
        } else {
            return 0;
        }
    }
}

void game_set_scroll_to_dave(game_context_t *game, dave_t *dave) {
    while (game_adjust_scroll_to_dave(game, dave) != 0) {};
}

int game_is_scrolling(game_context_t *game) {
    if (game->scroll_remaining != 0) {
        return 1;
    }

    return 0;
}

void game_do_bullets(game_context_t *game, tile_t *map, keys_state_t *keys)
{
    if (game->bullet != NULL) {
        game->bullet->tick(game->bullet, map, (game->scroll_offset * 16), (game->scroll_offset * 16) + 320);

        if (game->bullet->is_dead(game->bullet)) {
            game->bullet = NULL;
            printf("BULLET DEAD \n");
        }
    } else {
        if (keys->fire && game->dave->has_gun) {
            printf("Creating bullet!!\n");
            if (game->dave->face_direction == DAVE_DIRECTION_LEFT ||
                game->dave->face_direction == DAVE_DIRECTION_FRONTL) {
                game->bullet = bullet_create_left(game->dave->tile->x - 8, game->dave->tile->y + 8);
            } else if (game->dave->face_direction == DAVE_DIRECTION_RIGHT ||
                game->dave->face_direction == DAVE_DIRECTION_FRONTR) {
                game->bullet = bullet_create_right(game->dave->tile->x + 8, game->dave->tile->y + 8);
            }
        }
    }
}

void game_do_draw(game_context_t *game, tile_t *map, keys_state_t *keys) {
    int k;

    tile_t bottom_separator;
    tile_t top_separator;
    tile_t grail_banner;
    tile_t gun_banner;

    tile_create_bottom_separator(&bottom_separator, 0, 166);
    tile_create_top_separator(&top_separator, 0, 11);
    tile_create_grail_banner(&grail_banner, 70, 183);
    tile_create_gun_banner(&gun_banner, 240, 170);

    for (k = 0; k < 320 * 200; k++) {
        g_pixels[k] = 0x000000FF;
    }

    draw_map_offset(map, game->scroll_offset);
    draw_dave_offset(game->dave, g_assets, game->scroll_offset);
    if (game->monsters[0] != NULL) {
        draw_monster_offset(game->monsters[0], g_assets, game->scroll_offset);
    }
    if (game->bullet != NULL) {
        draw_bullet_offset(game->bullet, g_assets, game->scroll_offset);
    }
    draw_tile(&bottom_separator, g_assets);
    draw_tile(&top_separator, g_assets);
    if (game->dave->has_trophy) {
        draw_tile(&grail_banner, g_assets);
    }
    if (game->dave->has_gun) {
        draw_tile(&gun_banner, g_assets);
    }
    if (game->dave->jetpack_bars > 0) {
        draw_jetpack(game->dave->jetpack_bars);
    }
    draw_score(game->score);
    draw_level(game->level);
    draw_lives(game->lives);
}

/*
 * tile collision box calculation:
 *
 * y              +-----------------------------+
 *                |                             |
 * y+dy           |   +------------+            |
 *                |   |            |            |
 *                |   |            |            |
 *                |   |            |            |
 * (y+dy)+(h+dh)  |   +------------+            |
 *                |                             |
 * y+h            +-----------------------------+
 *                x  x+dx    (x+dx)+(w+dw)     x+w
 */
int collision_detect(tile_t *tile1, tile_t *tile2) {
    int box1_x = tile1->x + tile1->collision_dx;
    int box1_y = tile1->y + tile1->collision_dy;
    int box1_w = tile1->width + tile1->collision_dw;
    int box1_h = tile1->height + tile1->collision_dh;

    int box2_x = tile2->x + tile2->collision_dx;
    int box2_y = tile2->y + tile2->collision_dy;
    int box2_w = tile2->width + tile2->collision_dw;
    int box2_h = tile2->height + tile2->collision_dh;

    if (box1_x < box2_x + box2_w &&
        box1_x + box1_w > box2_x &&
        box1_y < box2_y + box2_h &&
        box1_y + box1_h > box2_y) {
        return 1;
    }

    return 0;
}

int game_level(game_context_t *game, tile_t *map, keys_state_t *keys) {
    dave_t *dave = game->dave;

    int i = 0;

    if (keys->quit) {
        printf("should quit! \n");
        return G_STATE_QUIT_NOW;
    }

    if (keys->escape) {
        g_soundfx->stop(g_soundfx);
        return G_STATE_LEVEL_POPUP;
    }


    // If we need to adjust screen by scrolling, just draw scene without progressing any
    // game objects.
    if (game_adjust_scroll_to_dave(game, game->dave)) {
        game_do_draw(game, map, keys);
        return G_STATE_LEVEL;
    }

    // Tick dave, monsters, and all block tiles in map
    game->dave->tick(game->dave, map, keys->left, keys->right, keys->jump, keys->down, keys->jetpack);
    if (game->monsters[0] != NULL) {
        game->monsters[0]->tick(game->monsters[0]);
    }

    for (i = 0; i < TILEMAP_WIDTH * TILEMAP_HEIGHT; i++) {
        if (map[i].sprites[0] != 0) {
            map[i].tick(&map[i]);
        }
    }

    game_do_bullets(game, map, keys);

    //collisions
    //check_dave_pick_item(game, map);
    /*if (game->dave->is_dead(game->dave)) {
        return;
    }*/

    if (dave->is_dead(game->dave)) {
        game->lives--;
        game->dave->state = DAVE_STATE_STANDING;
        game->dave->jump_state = 0;
        game->dave->step_count = 0;
        game->dave->on_fire = 0;

        if (game->lives == 0) {
            return G_STATE_GAMEOVER;
        }

        return G_STATE_LEVEL_START;
    }

    int idx = 0;
    for (idx = 0; idx < TILEMAP_WIDTH * TILEMAP_HEIGHT ; idx++) {
        if ((map[idx].sprites[0] != 0) &&
                collision_detect(game->dave->tile, &map[idx])) {

            if (map[idx].mod == LOOT) {
                game->score = game->score + map[idx].score_value;
                map[idx].sprites[0] = 0;
                map[idx].mod = 0;
                g_soundfx->play(g_soundfx, TUNE_TREASURE);

            } else if (map[idx].mod == TROPHY) {
                game->dave->has_trophy = 1;
                map[idx].mod = 0;
                map[idx].sprites[0] = 0;
                game->score = game->score + map[idx].score_value;
                g_soundfx->play(g_soundfx, TUNE_GOT_TROPHY);

            } else if (map[idx].mod == GUN) {
                game->dave->has_gun = 1;
                map[idx].mod = 0;
                map[idx].sprites[0] = 0;
                game->score = game->score + map[idx].score_value;
                g_soundfx->play(g_soundfx, TUNE_GOT_SOMETHING);

            } else if (map[idx].mod == JETPACK) {
                game->dave->jetpack_bars = 600;
                map[idx].mod = 0;
                map[idx].sprites[0] = 0;
                g_soundfx->play(g_soundfx, TUNE_GOT_SOMETHING);

            } else if (map[idx].mod == DOOR) {
                if (game->dave->has_trophy) {
                    g_soundfx->play(g_soundfx, TUNE_NEXTLEVEL);
                    return G_STATE_WARP_RIGHT;
                }

            } else if (map[idx].mod == FIRE) {
                if (game->dave->on_fire != 1) {
                    game->dave->on_fire = 1;
                    g_soundfx->stop(g_soundfx);
                    g_soundfx->play(g_soundfx, TUNE_OUCH);
                }
            } else if (map[idx].mod == CLIMB) {
                game->dave->on_tree = 1;
            }
        }
    }

    for (idx = 0; idx < 5; idx++) {
        if (game->monsters[idx] != NULL) {
            //printf("Checking collision for MONSTER %d \n", idx);
            //printf("monster[%d] x:%d y:%d \n", idx, game->monsters[idx]->tile->x, game->monsters[idx]->tile->y);
            if (game->bullet != NULL) {
                if (collision_detect(game->bullet->tile, game->monsters[idx]->tile)) {
                    if (game->monsters[idx]->on_fire != 1) {
                        printf("HIT \n");
                        game->bullet = NULL;
                        game->monsters[idx]->on_fire = 1;
                        g_soundfx->stop(g_soundfx);
                        g_soundfx->play(g_soundfx, TUNE_EXPLOSION);
                    }
                }
            }

            if (collision_detect(game->dave->tile, game->monsters[idx]->tile)) {
                if (game->monsters[idx]->is_alive(game->monsters[idx])) {
                    game->dave->on_fire = 1;
                    game->monsters[idx]->on_fire = 1;
                }
            }

/*            if (collision_detect(game->bullet->tile, game->monsters[idx]->tile)) {
                printf("MONSTER COLLISION \n");
            }*/
        }
    }

    game_do_draw(game, map, keys);
    return G_STATE_LEVEL;
}

int game_warp_right(game_context_t *game, tile_t *map, keys_state_t *keys) {
    int i = 0;
    int k = 0;

    tile_t bottom_separator;
    tile_t top_separator;
    tile_t grail_banner;

    tile_create_bottom_separator(&bottom_separator, 0, 166);
    tile_create_top_separator(&top_separator, 0, 11);
    tile_create_grail_banner(&grail_banner, 70, 180);

    if (keys->quit) {
        printf("should quit! \n");
        return G_STATE_QUIT_NOW;
    }

    if (keys->escape) {
        return G_STATE_LEVEL_POPUP;
    }
    game->dave->mute = 1;
    if  (game->dave->tile->x > 300) {
        game->level++;
        game->dave->mute = 0;
        return G_STATE_NONE;
    }

    game->dave->tick(game->dave, map, 0, 1, 0,0, 0);
    for (i = 0; i < TILEMAP_WIDTH * TILEMAP_HEIGHT; i++) {
        if (map[i].sprites[0] != 0) {
            map[i].tick(&map[i]);
        }
    }
    //check_dave_pick_item(game, map);

    for (k = 0; k < 320 * 200; k++) {
        g_pixels[k] = 0x000000FF;
    }
    // ================ draw map =====================
    for (k = 0; k < (TILEMAP_SCENE_WIDTH * TILEMAP_SCENE_HEIGHT); k++) {
        if (map[k + (game->scroll_offset * 12)].sprites[0] != 0) {
            draw_tile_offset(&map[k + (game->scroll_offset*12)], g_assets, game->scroll_offset);
        }
    }
    // ===============================================

    draw_x_levels_to_go(9 - game->level);
    draw_dave_offset(game->dave, g_assets, game->scroll_offset);
    draw_tile(&bottom_separator, g_assets);
    draw_tile(&top_separator, g_assets);
    if (game->dave->has_trophy) {
        draw_tile(&grail_banner, g_assets);
    }
    draw_score(game->score);
    draw_level(game->level);
    draw_lives(game->lives);

    return G_STATE_WARP_RIGHT;
}

int game_level_load(game_context_t *game, tile_t *map, char *file) {
    long fsize;
    char *buf;
    FILE* f = fopen(file, "rb");
    fseek(f, 0, SEEK_END);
    fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    buf = malloc(fsize + 1);
    fread(buf, 1, fsize, f);
    fclose(f);

    buf[fsize] = 0;

    int cur_col = 0;
    int pos = 0;

    int i = 0;
    int in_comment = 0;
    int collected_count = 0;
    char tag[4];
    tag[3] = 0;

    char *map_str = buf;
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
                    collected_count = 0;
                    if ((strcmp(tag, " D ") == 0) || (strcmp(tag, "D+M") == 0)) {
                        game->dave = dave_create(g_soundfx, cur_col * 16, pos * 16);
                        game->dave->state = DAVE_STATE_STANDING;
                        game->dave->jump_state = 0;
                        game->dave->step_count = 0;
                    } else if (strcmp(tag, "SUN") == 0) {
                        game->monsters[0] = monster_create_sun();
                        game->monsters[0]->tile->x = cur_col * 16;
                        game->monsters[0]->tile->y = pos * 20;
                        game->monsters[1] = NULL;
                    } else if (strcmp(tag, "SP1") == 0) {
                        game->monsters[0] = monster_create_spider();
                        game->monsters[0]->tile->x = cur_col * 16;
                        game->monsters[0]->tile->y = (pos * 20) - 12;
                        game->monsters[1] = NULL;
                    }

                    tile_create(&map[cur_col*12 + pos], tag, cur_col * 16, pos*16);
                    pos++;
                } else {
                    return -1;
                }
            } else if (map_str[i] == ';') {
                if (collected_count == 3) {
                    collected_count = 0;
                    tile_create(&map[cur_col*12 + pos], tag, cur_col*16, pos*16);
                    cur_col++;
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

int gameloop(int starting_level) {
    uint32_t timer_begin;
    uint32_t timer_end;
    uint32_t delay;
    game_context_t* game;
    tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT];
    keys_state_t key_state = {0, 0, 0, 0, 0, 0, 0, 0};
    tile_t flashing_cursor;
    char level_path[4096];
    int stride;
    int next_state;
    int g_state = G_STATE_NONE;
    int g_prev_state = G_STATE_NONE;


    game = malloc(sizeof(game_context_t));
    init_game(game);
    game->level = starting_level;
    tile_create_flashing_cursor(&flashing_cursor, 224, 96);

    while (1) {
        timer_begin = SDL_GetTicks();
        // SDL_SetRenderTarget(g_renderer, g_render_texture);
        SDL_SetRenderDrawColor(g_renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(g_renderer);
        SDL_LockTexture(g_texture, NULL, (void*)&g_pixels, &stride);

        get_keys(&key_state);

        if (g_state == G_STATE_NONE) {
            clear_monsters(game);
            clear_map(map);
            snprintf(level_path, 4096, "res/levels/level%ld.ddt", (long)game->level);

            game_level_load(game, map, level_path);
            next_state = G_STATE_LEVEL_START;

        } else if (g_state == G_STATE_LEVEL_START) {
            printf("Resetting dave location \n");
            game->dave->tile->x = game->dave->tile->default_x;
            game->dave->tile->y = game->dave->tile->default_y;
            game->scroll_offset = 0;
            game_set_scroll_to_dave(game, game->dave);
            next_state = G_STATE_LEVEL;

        } else if (g_state == G_STATE_LEVEL) {
            next_state = game_level(game, map, &key_state);

        } else if (g_state == G_STATE_LEVEL_POPUP) {
            next_state = game_popup_quit_routine(game, map, &flashing_cursor, &key_state);

        } else if (g_state == G_STATE_WARP_RIGHT) {
            if (g_prev_state != G_STATE_WARP_RIGHT) {
                game->dave->has_trophy = 0;
                game->dave->step_count = 0;
                game->dave->state = DAVE_STATE_STANDING;
                game->dave->has_gun = 0;
                game->dave->jetpack_bars = 0;
                game->scroll_offset = 0;
                clear_map(map);
                game_level_load(game, map, "res/levels/warp_right.ddt");
            }
            next_state = game_warp_right(game, map, &key_state);

        } else if (g_state == G_STATE_GAMEOVER) {
            printf("GAME OVER!!! \n");
            SDL_UnlockTexture(g_texture);
            return 2;

        } else if (g_state == G_STATE_QUIT_NOW) {
            SDL_UnlockTexture(g_texture);
            return 1;

        } else {

        }

        g_prev_state = g_state;
        g_state = next_state;

        SDL_UnlockTexture(g_texture);
        SDL_RenderCopy(g_renderer, g_texture, NULL, NULL);
        SDL_RenderPresent(g_renderer);

        timer_end = SDL_GetTicks();
        delay = 14 - (timer_end-timer_begin);
        delay = delay > 14 ? 0 : delay;

        SDL_Delay(delay);
    }

    return 0;
}

void sigseg_handler(int sig) {
    void *array[10];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 10);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}

int game_main(int is_windowed, int starting_level) {
    int ret = 0;
    signal(SIGSEGV, sigseg_handler);
    SDL_AudioSpec audio_spec_want, audio_spec;
    const uint8_t DISPLAY_SCALE = 3;

    SDL_zero(audio_spec_want);
    SDL_zero(audio_spec);

    // Initialize SDL
    SDL_SetMainReady();
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE)) {
        printf("Failed to initialize SDL video. Error: (%s) \n", SDL_GetError());
        return -1;
    }

    if (SDL_Init(SDL_INIT_AUDIO)) {
        printf("Failed to initialize SDL audio. Error: (%s) \n", SDL_GetError());
        return -2;
    }

    if (is_windowed) {
        printf("creating SDL window \n");
        g_window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 600,  0 );
    } else {
        g_window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 960, 600,  SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_RESIZABLE );
    }
    printf("creating renderer \n");
    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_SOFTWARE);
    g_texture = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 320, 200);

    SDL_RenderSetScale(g_renderer, DISPLAY_SCALE, DISPLAY_SCALE);

    // Flush any pre-pressed keys
    SDL_FlushEvent(SDL_KEYDOWN);
    SDL_FlushEvent(SDL_MOUSEBUTTONDOWN);
    SDL_FlushEvent(SDL_MOUSEMOTION);

    printf("initializing assets \n");
    g_assets = malloc(sizeof(struct game_assets));
    init_assets(g_assets, g_renderer);

    g_soundfx = soundfx_create();

    while (1) {
        ret = start_intro();
        ret = gameloop(starting_level);
        printf("game-loop finished with ret-code: %d \n", ret);

        if (ret == 0) {
            return 0;
        } else if (ret == 1) {
            printf("bye bye \n");
            soundfx_destroy(g_soundfx);
            g_soundfx = NULL;
            SDL_Quit();
            return 0;
        } else if (ret == 2) {
            printf("game-over \n");
        }
    }

    soundfx_destroy(g_soundfx);
    g_soundfx = NULL;
    SDL_Quit();
    return 0;
}



