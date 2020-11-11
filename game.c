#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <SDL.h>

#include "game.h"

#define DEBUG_INPUT 1



SDL_Renderer* g_renderer;
SDL_Texture* g_render_texture;
font_t* g_font;
assets_t* g_assets;

void draw_tile(tile_t* tile, struct game_assets* assets);
void draw_char(char c, int x, int y, font_t* font, SDL_Renderer* renderer);
void draw_text_line(const char* line, int x, int y, font_t* font, SDL_Renderer* renderer);

void draw_text_line_black(const char* line, int x, int y, font_t* font, SDL_Renderer* renderer);
void draw_dave(dave_t *dave, struct game_assets* assets);

void draw_score(int score);

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
                } else if (col == (columns-1)) {
                    cur_sprite = SPRITE_IDX_POPUP_BOX_T3;
                } else {
                    cur_sprite = SPRITE_IDX_POPUP_BOX_T2;
                }
            } else if (row == (rows -1)) {
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
                } else if (col == (columns-1)) {
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

/* Bring in tileset from tile<xxx>.bmp files from original binary (see TILES.C)*/
void init_assets(struct game_assets *assets, SDL_Renderer *renderer)
{
    int i,j;
    char fname[50];
    char file_num[4];
    char mname[50];
    char mask_num[4];
    SDL_Surface *surface;
    SDL_Surface *mask;
    uint8_t *surf_p;
    uint8_t *mask_p;
    uint8_t mask_offset;

    for (i=0; i<173; i++)
    {
        fname[0]='\0';
        strcat(fname, "res/tiles/tile");
        sprintf(&file_num[0],"%u",i);
        strcat(fname, file_num);
        strcat(fname, ".bmp");

        /* Handle Dave tile masks */
        if ((i >= 53 && i <= 59) || i == 67 || i == 68 || (i >= 71 && i <= 73) || (i >= 77 && i <= 82)) {
            if (i >= 53 && i <= 59)
                mask_offset = 7;
            if (i >= 67 && i <= 68)
                mask_offset = 2;
            if (i >= 71 && i <= 73)
                mask_offset = 3;
			if (i >= 77 && i <= 82)
				mask_offset = 6;

			mname[0]='\0';
			strcat(mname, "res/tiles/tile");
			sprintf(&mask_num[0],"%u",i+mask_offset);
			strcat(mname, mask_num);
			strcat(mname, ".bmp");

			surface = SDL_LoadBMP(fname);
			mask = SDL_LoadBMP(mname);

			surf_p = (uint8_t*) surface->pixels;
			mask_p = (uint8_t*) mask->pixels;

			/* Write mask white background to dave tile */
			for (j=0;j<(mask->pitch*mask->h);j++)
				surf_p[j] = mask_p[j] ? 0xFF : surf_p[j];

			/* Make white mask transparent */
			SDL_SetColorKey(surface, 1, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
			assets->graphics_tiles[i] = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_FreeSurface(surface);
			SDL_FreeSurface(mask);
		}
		else {
            surface = SDL_LoadBMP(fname);

			/* Monster tiles should use black transparency */
			if ((i >= 89 && i <= 120 ) || (i >= 129 && i <= 132 ))
				SDL_SetColorKey(surface, 1, SDL_MapRGB(surface->format, 0x00, 0x00, 0x00));
			assets->graphics_tiles[i] = SDL_CreateTextureFromSurface(renderer, surface);
		}
	}

}


/* Set game and monster properties to default values */
void init_game(game_context_t *game)
{
    int i,j;
    FILE *file_level;
    char fname[100];
    char file_num[4];

    game->quit = 0;
    game->tick = 0;
    game->current_level = 0;
    game->lives = 3;
    game->score = 0;
    game->view_x = 0;
    game->view_y = 0;
    game->scroll_x = 0;
    game->try_right = 0;
    game->try_left = 0;
    game->try_jump = 0;
    game->try_fire = 0;
    game->try_jetpack = 0;
    game->try_down = 0;
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

    game->dave = dave_create2();
    game->dave->tile->x = 20;
    game->dave->tile->y = 40;
    game->dave->tile->width = 20;
    game->dave->tile->height = 16;
    game->dave->state = DAVE_STATE_STANDING;
    game->dave->jump_state = 0;
    game->dave->step_count = 0;

    for (j=0;j<5;j++)
        game->monster[j].type = 0;

    /* Load each level from level<xxx>.dat. (see LEVEL.c utility) */
    for (j=0; j<10; j++)
    {
        fname[0]='\0';
        strcat(fname, "res/levels/level");
        sprintf(&file_num[0],"%u",j);
        strcat(fname, file_num);
        strcat(fname, ".dat");

        file_level = fopen(fname, "rb");

        for (i=0; i<sizeof(game->level[j].path); i++)
            game->level[j].path[i] = fgetc(file_level);

        for (i=0; i<sizeof(game->level[j].tiles); i++)
            game->level[j].tiles[i] = fgetc(file_level);

        for (i=0; i<sizeof(game->level[j].padding); i++)
            game->level[j].padding[i] = fgetc(file_level);

        fclose(file_level);
    }
}

void check_input2(keys_state_t* state)
{
    SDL_Event event;
    SDL_PollEvent(&event);
    const uint8_t *keystate = SDL_GetKeyboardState(NULL);
    state->right   = (keystate[SDL_SCANCODE_RIGHT] != 0) ? 1 : 0;
    state->left    = (keystate[SDL_SCANCODE_LEFT]  != 0) ? 1 : 0;
    state->jump    = (keystate[SDL_SCANCODE_UP]    != 0) ? 1 : 0;
    state->down    = (keystate[SDL_SCANCODE_DOWN]  != 0) ? 1 : 0;
    state->fire    = (keystate[SDL_SCANCODE_LCTRL] != 0) ? 1 : 0;
    state->jetpack = (keystate[SDL_SCANCODE_LALT]  != 0) ? 1 : 0;
    state->quit    = (event.type == SDL_QUIT) ? 1 : 0;
}

/* Clear flags set by keyboard input */
void clear_input(game_context_t *game) {
    game->try_jump = 0;
    game->try_right = 0;
    game->try_left = 0;
    game->try_fire = 0;
    game->try_jetpack = 0;
    game->try_down = 0;
    game->try_up = 0;
}

void show_quit_popup() {
    uint32_t timer_begin;
    uint32_t timer_end;
    uint32_t delay;
    SDL_Event event;
    int popup_should_finish = 0;
    tile_t flashing_cursor;
    tile_create_flashing_cursor(&flashing_cursor, 224, 96);

    printf("Starting Quit Popup \n");
    while (!popup_should_finish) {
        timer_begin = SDL_GetTicks();

        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                printf("SDL_QUIT \n");
                exit(0);
            }

            const uint8_t *keystate = SDL_GetKeyboardState(NULL);
            if (keystate[SDL_SCANCODE_ESCAPE]) {
                //printf("ESC \n");
                //show_quit_popup();	
            }

            if ( keystate[SDL_SCANCODE_RIGHT] ||
                keystate[SDL_SCANCODE_LEFT] ||
                keystate[SDL_SCANCODE_UP] ||
                keystate[SDL_SCANCODE_DOWN] ||
                keystate[SDL_SCANCODE_RETURN] ||
                keystate[SDL_SCANCODE_SPACE]) {
                popup_should_finish = 1;
            }

            if ( keystate[SDL_SCANCODE_N]) {
                popup_should_finish = 1;
            }
        }

        SDL_SetRenderTarget(g_renderer, g_render_texture);
        // Draw all tiles
        draw_popup_box(88, 80, 5, 21);
        draw_text_line_black("QUIT? (Y OR N):", 104, 98, g_font, g_renderer);
        draw_tile(&flashing_cursor, g_assets);
        flashing_cursor.tick(&flashing_cursor);

        // Set the screen as the target of renderer
        SDL_SetRenderTarget(g_renderer, NULL);

        // Clear the render (clear screen)
        SDL_RenderClear(g_renderer);

        // Render texture into screen
        SDL_RenderCopy(g_renderer, g_render_texture, NULL,NULL);

        // Swaps display buffers (puts above drawing on the screen)
        SDL_RenderPresent(g_renderer);

        // Swaps display buffers (puts above drawing on the screen)
        //SDL_RenderPresent(g_renderer);

        timer_end = SDL_GetTicks();
        delay = 14 - (timer_end-timer_begin);
        delay = delay > 14 ? 0 : delay;
        printf("delay: %d \n", delay);
        SDL_Delay(delay);
    }
}

// returns 0 if key pressed, 1 if timeout
int start_intro() {
    int k;
    int32_t intro_should_finish = 0;
    uint32_t timer_begin;
    uint32_t timer_end;
    uint32_t delay;
    game_context_t *game;
    int32_t result = 0;
    SDL_Event event;


    /* Clear screen */
    SDL_SetRenderDrawColor(g_renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(g_renderer);

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

    // Game loop with fixed time step at 30 FPS
    while (!intro_should_finish) {
        timer_begin = SDL_GetTicks();

        // All next drawings will be done into the texture (as oppose to screen)
        while(SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT) {
                printf("SDL_QUIT \n");
                exit(0);
            }
            const uint8_t *keystate = SDL_GetKeyboardState(NULL);
            if (keystate[SDL_SCANCODE_ESCAPE]) {
                printf("ESC \n");
                show_quit_popup();
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
        }

        SDL_SetRenderTarget(g_renderer, g_render_texture);
        SDL_SetRenderDrawColor(g_renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(g_renderer);

        // Draw all tiles
        for (k = 0; k < 41; k++) {
            draw_tile(&block[k], g_assets);
            block[k].tick(&block[k]);
        }

        draw_text_line("BY JOHN ROMERO", 110, 50, g_font, g_renderer);
        draw_text_line("(C) 1990 SOFTDISK, INC.", 79, 57, g_font, g_renderer);
        draw_text_line("                         ",72, 166, g_font, g_renderer);
        draw_text_line("PRESS THE F1 KEY FOR HELP", 72, 168, g_font, g_renderer);
        draw_text_line("                         ",72, 174, g_font, g_renderer);

        // Set the screen as the target of renderer
        SDL_SetRenderTarget(g_renderer, NULL);

        // Clear the render (clear screen)
        SDL_RenderClear(g_renderer);

        // Render texture into screen
        SDL_RenderCopy(g_renderer, g_render_texture, NULL,NULL);

        // Swaps display buffers (puts above drawing on the screen)
        SDL_RenderPresent(g_renderer);

        timer_end = SDL_GetTicks();
        delay = 14 - (timer_end-timer_begin);
        delay = delay > 14 ? 0 : delay;

        SDL_Delay(delay);
    }

    /* Clean up and quit */
    free(game);

    return result;
}

int is_tile_empty(tile_t *tile) {
    if (tile->mod == 0) {
        return 1;
    }

    return 0;
}

int is_dave_collision_tile(dave_t *dave, tile_t *tile) {
    if (is_tile_empty(tile)) {
        return 0;
    }

    if (tile->is_inside(tile, dave->tile->x-2, dave->tile->y+2)) {
        return 1;
    }
    if (tile->is_inside(tile, dave->tile->x-2, dave->tile->y+12)) {
        return 1;
    }
    if (tile->is_inside(tile, dave->tile->x+14, dave->tile->y+2)) {
        return 1;
    }
    if (tile->is_inside(tile, dave->tile->x+14, dave->tile->y+12)) {
        return 1;
    }

    return 0;
}

int is_dave_in_door(game_context_t *game, tile_t *map) {
    if (game->dave->is_dead) {
        return 0;
    }

    int idx = 0;
    for (idx = 0; idx < TILEMAP_WIDTH * TILEMAP_HEIGHT ; idx++) {
        if (is_dave_collision_tile(game->dave, &map[idx])) {
            if (map[idx].mod == DOOR) {
                return 1;
            }
        }
    }

    return 0;
}

void check_dave_pick_item(game_context_t *game, tile_t *map) {
    if (game->dave->is_dead) {
        return;
    }
    int idx = 0;
    for (idx = 0; idx < TILEMAP_WIDTH * TILEMAP_HEIGHT ; idx++) {
        if (is_dave_collision_tile(game->dave, &map[idx])) {
            if (map[idx].mod == ITEM) {
                map[idx].mod = 0;
                map[idx].sprites[0] = 0;
                return;
            } else if (map[idx].mod == GRAIL) {
                game->dave->has_grail = 1;
                map[idx].mod = 0;
                map[idx].sprites[0] = 0;
            }
        }
    }
}

int check_dave_touch_fire(game_context_t *game, tile_t *map) {
    if (game->dave->is_dead) {
        return 0;
    }

    int idx = 0;
    for (idx = 0; idx < TILEMAP_WIDTH * TILEMAP_HEIGHT ; idx++) {
        if (is_dave_collision_tile(game->dave, &map[idx])) {
            if (map[idx].mod == FIRE) {
                return 1;
            }
        }
    }

    return 0;
}

int start_warp_right() {
    uint32_t timer_begin;
    uint32_t timer_end;
    uint32_t delay;

    game_context_t* game;
    keys_state_t key_state = {0, 0, 0, 0, 0, 0, 0};
    int should_quit = 0;
    int i, j;
    tile_t bottom_separator;
    tile_t top_separator;
    tile_t grail_banner;

    // Initialize game state
    game = malloc(sizeof(game_context_t));
    init_game(game);
    tile_create_bottom_separator(&bottom_separator, 0, 166);
    tile_create_top_separator(&top_separator, 0, 11);
    tile_create_grail_banner(&grail_banner, 70, 180);

    // Start level 1
    tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT];
    for (i = 0; i < TILEMAP_WIDTH * TILEMAP_HEIGHT; i++){
        map[i].sprites[0] = 0;
        map[i].mod = 0;
        map[i].x = 0;
        map[i].y = 0;
    }
    tile_file_parse(map, "res/levels/warp_right.ndt");
    printf("map0 x,y: %d, %d \n", map[0].x, map[0].y);

    game->dave->tile->x = 40;
    game->dave->tile->y = 80;

    while (!should_quit) {
        timer_begin = SDL_GetTicks();

        check_input2(&key_state);
        key_state.jump = 0;
        key_state.left = 0;
        key_state.right = 1;
        if (key_state.quit) {
            should_quit = 1;
        }
        printf("dave: [%d,%d] \n", game->dave->tile->x, game->dave->tile->y);
        if (game->dave->tile->x > 300) {
            return 1;
        }

        game->dave->tick(game->dave, map, key_state.left, key_state.right, key_state.jump);
        for (i = 0; i < TILEMAP_WIDTH * TILEMAP_HEIGHT; i++) {
            if (map[i].sprites[0] != 0)
                map[i].tick(&map[i]);
        }
        check_dave_pick_item(game, map);
        SDL_SetRenderTarget(g_renderer, g_render_texture);
        SDL_SetRenderDrawColor(g_renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(g_renderer);

        // ================ draw map =====================
        for (i=TILEMAP_SCENE_Y; i<TILEMAP_SCENE_HEIGHT; i++) {
            for (j=TILEMAP_SCENE_X; j < TILEMAP_SCENE_WIDTH; j++) {
                if (map[i*20 + j].sprites[0] != 0) {
                    draw_tile(&map[i*20 + j], g_assets);
                }
            }
        }
        // ===============================================

        // ================ draw dave ===================
        draw_dave(game->dave, g_assets);
        // ==============================================

        // ============== draw misc items ================
        draw_tile(&bottom_separator, g_assets);
        draw_tile(&top_separator, g_assets);
        if (game->dave->has_grail) {
            draw_tile(&grail_banner, g_assets);
        }
        draw_score(500);
        // ===============================================


        draw_text_line("GOOD WORK! ONLY 9 MORE TO GO!", 50, 58, g_font, g_renderer);

        // Set the screen as the target of renderer
        SDL_SetRenderTarget(g_renderer, NULL);

        // Clear the render (clear screen)
        SDL_RenderClear(g_renderer);

        // Render texture into screen
        SDL_RenderCopy(g_renderer, g_render_texture, NULL,NULL);

        // Swaps display buffers (puts above drawing on the screen)
        SDL_RenderPresent(g_renderer);
        timer_end = SDL_GetTicks();

        delay = 14 - (timer_end-timer_begin);
        delay = delay > 14 ? 0 : delay;
        //printf("DELAY=%d \n", delay);
        SDL_Delay(delay);
        if (game->quit) {
            should_quit = 1;
        }
    }

    return 0;
}

int start_level(int level) {
    uint32_t timer_begin;
    uint32_t timer_end;
    uint32_t delay;

    game_context_t* game;
    keys_state_t key_state = {0, 0, 0, 0, 0, 0, 0};
    int should_quit = 0;
    int i, j;
    tile_t bottom_separator;
    tile_t top_separator;
    tile_t grail_banner;

    // Initialize game state
    game = malloc(sizeof(game_context_t));
    init_game(game);
    tile_create_bottom_separator(&bottom_separator, 0, 166);
    tile_create_top_separator(&top_separator, 0, 11);
    tile_create_grail_banner(&grail_banner, 70, 180);

    // Start level 1
    tile_t map[TILEMAP_WIDTH * TILEMAP_HEIGHT];
    for (i = 0; i < TILEMAP_WIDTH * TILEMAP_HEIGHT; i++){
        map[i].sprites[0] = 0;
        map[i].mod = 0;
        map[i].x = 0;
        map[i].y = 0;
    }

    tile_file_parse(map, "res/levels/level1.ndt");
    printf("map0 x,y: %d, %d \n", map[0].x, map[0].y);

    game->dave->tile->x = 40;
    game->dave->tile->y = 144;

    while (!should_quit) {
        timer_begin = SDL_GetTicks();

        check_input2(&key_state);
        if (key_state.quit) {
            printf("should quit! \n");
            should_quit = 1;
        }

        game->dave->tick(game->dave, map, key_state.left, key_state.right, key_state.jump);
        for (i = 0; i < TILEMAP_WIDTH * TILEMAP_HEIGHT; i++) {
            if (map[i].sprites[0] != 0)
                map[i].tick(&map[i]);
        }
        check_dave_pick_item(game, map);
        if (is_dave_in_door(game, map)) {
            printf("IN DOOR \n");
            printf("IN DOOR \n");
            printf("IN DOOR \n");
            printf("IN DOOR \n");
            return 1;
        }
        if (check_dave_touch_fire(game, map)) {
            printf("Dave is dead \n");
            game->dave->is_dead = 1;
        }

        if (game->dave->is_dead) {
            printf("dave died %d ticks before\n", game->dave->ticks_since_dead);
            if (game->dave->ticks_since_dead > 200) {
                return 2;
            }
        }

        SDL_SetRenderTarget(g_renderer, g_render_texture);
        SDL_SetRenderDrawColor(g_renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(g_renderer);

        // ================ draw map =====================
        for (i=TILEMAP_SCENE_Y; i<TILEMAP_SCENE_HEIGHT; i++) {
            for (j=TILEMAP_SCENE_X; j < TILEMAP_SCENE_WIDTH; j++) {
                if (map[i*20 + j].sprites[0] != 0) {
                    draw_tile(&map[i*20 + j], g_assets);
                }
            }
        }
        // ===============================================

        // ================ draw dave ===================
        draw_dave(game->dave, g_assets);
        // ==============================================
        //
        // ============== draw misc items ================
        draw_tile(&bottom_separator, g_assets);
        draw_tile(&top_separator, g_assets);
        if (game->dave->has_grail) {
            draw_tile(&grail_banner, g_assets);
        }
        draw_score(500);
        // ===============================================


        // Set the screen as the target of renderer
        SDL_SetRenderTarget(g_renderer, NULL);

        // Clear the render (clear screen)
        SDL_RenderClear(g_renderer);

        // Render texture into screen
        SDL_RenderCopy(g_renderer, g_render_texture, NULL,NULL);

        // Swaps display buffers (puts above drawing on the screen)
        SDL_RenderPresent(g_renderer);
        timer_end = SDL_GetTicks();

        delay = 14 - (timer_end-timer_begin);
        delay = delay > 14 ? 0 : delay;
        //printf("DELAY=%d \n", delay);
        SDL_Delay(delay);
        if (game->quit) {
            should_quit = 1;
        }
    }

    return 0;
}

int main(int argc, char* argv[])
{
    int ret = 0;
    int level = 0;
    const uint8_t DISPLAY_SCALE = 2;
    SDL_Window* g_window;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE)) {
        printf("Failed to initialize SDL video. Error: (%s) \n", SDL_GetError());
        return -1;
    }
    printf("creating SDL window \n");
    g_window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 400, 0);

    printf("creating renderer \n");
    //g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_SOFTWARE | SDL_RENDERER_TARGETTEXTURE);
    //g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_SOFTWARE);
    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
    printf("creating textures \n");
    g_render_texture = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 320, 200);

    // Easy onversion between original world (320x200) and current screen size
    SDL_RenderSetScale(g_renderer, DISPLAY_SCALE, DISPLAY_SCALE);

    printf("initializing assets \n");
    g_assets = malloc(sizeof(struct game_assets));
    init_assets(g_assets, g_renderer);

    printf("loading font \n");
    g_font = font_create(g_renderer);

    printf("starting intro  \n");
    ret = start_intro();
    printf("intro ret: %d \n", ret);

    if (ret == -1) {
        printf("game ended1 \n");
        SDL_Quit();
        return 0;
    }

    start_level(0);
/*    while (level <= 5) {
        ret = start_level(level);

        if (ret == 1) {
            ret = start_warp_right();
            level++;
        } else if (ret == 2) {
            //reduce 1 life`
        } else if (ret == 3) {
            //exit
        }
    }*/
    SDL_Quit();

    return 0;
}

void draw_text_line(const char* line, int x, int y, font_t* font, SDL_Renderer* renderer) {
    int i = 0;
    for (i = 0; i < strlen(line); i++) {
        draw_char(line[i], x + (i*8), y, font, renderer);
    }
}

void draw_char_black(char c, int x, int y, font_t* font, SDL_Renderer* renderer) {
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = 8;
    dest.h = 6;
    SDL_RenderCopy(renderer, font->black[(int)c], NULL, &dest);
}

void draw_text_line_black(const char* line, int x, int y, font_t* font, SDL_Renderer* renderer) {
    int i = 0;
    for (i = 0; i < strlen(line); i++) {
        draw_char_black(line[i], x + (i*8), y, font, renderer);
    }
}

void draw_char(char c, int x, int y, font_t* font, SDL_Renderer* renderer) {
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = 8;
    dest.h = 6;
    SDL_RenderCopy(renderer, font->white[(int)c], NULL, &dest);
}

void draw_score(int score) {
    int i;
    SDL_Rect dest;
    dest.x = 0;
    dest.y = 0;
    dest.w = 62;
    dest.h = 11;
    SDL_RenderCopy(g_renderer, g_assets->graphics_tiles[137], NULL, &dest);

    SDL_Rect digit_dest;
    digit_dest.x = 64;
    digit_dest.y = 1;
    digit_dest.w = 8;
    digit_dest.h = 11;

    for (i = 0; i < 5; i++) {
        SDL_RenderCopy(g_renderer, g_assets->graphics_tiles[148], NULL, &digit_dest);
        digit_dest.x = digit_dest.x + 8;
    }
}

void draw_dave(dave_t *dave, struct game_assets* assets) {
    SDL_Rect dest;
    int sprite;
    dest.x = dave->tile->x;
    dest.y = dave->tile->y;
    dest.w = dave->tile->width;
    dest.h = dave->tile->height;

    sprite = dave->tile->get_sprite(dave->tile);
    SDL_RenderCopy(g_renderer, assets->graphics_tiles[sprite], NULL, &dest);
}

void draw_tile(tile_t* tile, struct game_assets* assets)
{
    SDL_Rect dest;

    dest.x = tile->x;
    dest.y = tile->y;
    dest.w = tile->width;
    dest.h = tile->height;
//	printf("drawing tile of size W:%d, H:%d, into (%d,%d)\n", dest.w, dest.h, dest.x,dest.y);
    SDL_RenderCopy(g_renderer, assets->graphics_tiles[tile->get_sprite(tile)], NULL, &dest); 
}

