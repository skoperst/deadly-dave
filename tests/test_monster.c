#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "game.h"

SDL_Window* g_window;
SDL_Renderer* g_renderer;
SDL_Texture *g_texture;
uint32_t *g_pixels;
assets_t* g_assets;


void do_input(keys_state_t* state)
{
    SDL_Event event;

    const uint8_t *keystate = SDL_GetKeyboardState(NULL);
    state->right      = (keystate[SDL_SCANCODE_RIGHT]  != 0) ? 1 : 0;
    state->left       = (keystate[SDL_SCANCODE_LEFT]   != 0) ? 1 : 0;
    state->jump       = (keystate[SDL_SCANCODE_UP]     != 0) ? 1 : 0;
    state->down       = (keystate[SDL_SCANCODE_DOWN]   != 0) ? 1 : 0;
    state->escape     = (keystate[SDL_SCANCODE_ESCAPE] != 0) ? 1 : 0;
    state->fire       = (keystate[SDL_SCANCODE_LCTRL]  != 0) ? 1 : 0;
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
void render_tile_idx(int tile_idx, int x, int y) {
    SDL_Surface *surface = g_assets->tiles[tile_idx];

    int blend = 0;
    if (tile_idx == SPRITE_IDX_BULLET_RIGHT || tile_idx == SPRITE_IDX_BULLET_LEFT) {
        blend = 1;
    }

    int column_idx, line_idx;
    for (line_idx = 0; line_idx < surface->h; line_idx++) {
        for (column_idx = 0; column_idx < surface->w; column_idx++) {
            uint32_t pixel = ((uint32_t*)surface->pixels)[line_idx * surface->w + column_idx];

            if ( (pixel & 0x000000FF) == 0) { // is a pixel totally transperant dont draw it
            } else if (line_idx + y >= 200) {
            } else if (column_idx + x >= 320) {
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

void draw_monster_offset(monster_t *monster, struct game_assets *assets, int x_offset) {
    if (monster->tile->get_sprite(monster->tile) != 0) {
        render_tile_idx(monster->tile->get_sprite(monster->tile),
            monster->tile->x - (x_offset * 16), monster->tile->y);
    }
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


int test_monster() {
    int32_t intro_should_finish = 0;
    uint32_t timer_begin;
    uint32_t timer_end;
    uint32_t delay;
    int32_t result = 0;
    int stride;
    int idx;


    keys_state_t key_state = {0, 0, 0, 0, 0, 0, 0, 0};
    memset(&key_state, 0x00, sizeof(keys_state_t));

    tile_t blockz[40];
    tile_t bottom_separator;

    tile_create_block(&blockz[0], SPRITE_IDX_DIRT, 0, 16, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[1], SPRITE_IDX_DIRT, 16, 16, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[2], SPRITE_IDX_DIRT, 32, 16, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[3], SPRITE_IDX_DIRT, 48, 16, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[4], SPRITE_IDX_DIRT, 64, 16, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[5], SPRITE_IDX_DIRT, 80, 16, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[6], SPRITE_IDX_DIRT, 96, 16, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[7], SPRITE_IDX_DIRT, 112, 16, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[8], SPRITE_IDX_DIRT, 128, 16, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[9], SPRITE_IDX_DIRT, 144, 16, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[10], SPRITE_IDX_DIRT, 160, 16, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[11], SPRITE_IDX_DIRT, 176, 16, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[12], SPRITE_IDX_DIRT, 192, 16, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[13], SPRITE_IDX_DIRT, 208, 16, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[14], SPRITE_IDX_DIRT, 224, 16, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[15], SPRITE_IDX_DIRT, 240, 16, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[16], SPRITE_IDX_DIRT, 256, 16, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[17], SPRITE_IDX_DIRT, 272, 16, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[18], SPRITE_IDX_DIRT, 288, 16, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[19], SPRITE_IDX_DIRT, 304, 16, TILE_SIZE, TILE_SIZE);

    tile_create_block(&blockz[20], SPRITE_IDX_RED_BRICK, 0, 160, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[21], SPRITE_IDX_RED_BRICK, 16, 160, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[22], SPRITE_IDX_RED_BRICK, 32, 160, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[23], SPRITE_IDX_RED_BRICK, 48, 160, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[24], SPRITE_IDX_RED_BRICK, 64, 160, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[25], SPRITE_IDX_RED_BRICK, 80, 160, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[26], SPRITE_IDX_RED_BRICK, 96, 160, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[27], SPRITE_IDX_RED_BRICK, 112, 160, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[28], SPRITE_IDX_RED_BRICK, 128, 160, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[29], SPRITE_IDX_RED_BRICK, 144, 160, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[30], SPRITE_IDX_RED_BRICK, 160, 160, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[31], SPRITE_IDX_RED_BRICK, 176, 160, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[32], SPRITE_IDX_RED_BRICK, 192, 160, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[33], SPRITE_IDX_RED_BRICK, 208, 160, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[34], SPRITE_IDX_RED_BRICK, 224, 160, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[35], SPRITE_IDX_RED_BRICK, 240, 160, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[36], SPRITE_IDX_RED_BRICK, 256, 160, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[37], SPRITE_IDX_RED_BRICK, 272, 160, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[38], SPRITE_IDX_RED_BRICK, 288, 160, TILE_SIZE, TILE_SIZE);
    tile_create_block(&blockz[39], SPRITE_IDX_RED_BRICK, 304, 160, TILE_SIZE, TILE_SIZE);

    tile_create_bottom_separator(&bottom_separator, 0, 166);

    monster_t *mtt = monster_create_swirl();
    mtt->tile->x = 200;
    mtt->tile->y = 50;

    while (!intro_should_finish) {
        timer_begin = SDL_GetTicks();

        do_input(&key_state);

        if (key_state.escape) {
            return 0;
        } else if (key_state.enter) {
            key_state.enter = 0;
        }

        // Clear screen
        SDL_SetRenderDrawColor(g_renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(g_renderer);
        SDL_LockTexture(g_texture, NULL, (void*)&g_pixels, &stride);


        for (idx = 0; idx < 320 * 200; idx++) {
            g_pixels[idx] = 0x000000FF;
        }


        for (idx = 0; idx < (sizeof(blockz) / sizeof(blockz[0])); idx++) {
            draw_tile(&blockz[idx], g_assets);
        }
        draw_monster_offset(mtt, g_assets, 0);
        draw_tile(&bottom_separator, g_assets);
        draw_text_line("PRESS ESCAPE TO EXIT", 72, 6, g_renderer);

        // Tick
        for (idx = 0; idx < (sizeof(blockz) / sizeof(blockz[0])); idx++) {
            blockz[idx].tick(&blockz[idx]);
        }
        mtt->tick(mtt);


        // Refresh screen
        SDL_UnlockTexture(g_texture);
        SDL_RenderCopy(g_renderer, g_texture, NULL,NULL);
        SDL_RenderPresent(g_renderer);

        // Wait for next frame
        timer_end = SDL_GetTicks();
        delay = 14 - (timer_end-timer_begin);
        delay = delay > 14 ? 0 : delay;
        SDL_Delay(delay);
    }

    return result;
}

void load_assets() {
    int i;
    char fname[50];
    char file_num[4];
    SDL_Surface *surface;

    g_assets = malloc(sizeof(struct game_assets));

    for (i = 0; i < 1000; i++) {
        g_assets->tiles[i] = NULL;
    }

    for (i=0; i<1000; i++) {
        fname[0]='\0';
        strcat(fname, "../res/tiles/tile");
        sprintf(&file_num[0],"%u",i);
        strcat(fname, file_num);
        strcat(fname, ".bmp");

        if (access(fname, F_OK) == 0) {
            surface = SDL_LoadBMP(fname);
            g_assets->tiles[i] = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
        }
    }
}

int main(int argc, char* argv[]) {
    int ret = 0;
    const uint8_t DISPLAY_SCALE = 3;

    SDL_SetMainReady();
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE)) {
        printf("Failed to initialize SDL video. Error: (%s) \n", SDL_GetError());
        return -1;
    }

    g_window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 600, 0);
    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_SOFTWARE);
    g_texture = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING, 320, 200);

    SDL_RenderSetScale(g_renderer, DISPLAY_SCALE, DISPLAY_SCALE);

    load_assets();

    ret = test_monster();
    SDL_Quit();

    return ret;
}



