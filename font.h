#ifndef _FONT_H_
#define _FONT_H_

#include <SDL.h>

typedef struct font_struct {
    SDL_Texture* letter_texture[256];
    SDL_Texture* black[256];
} font_t;

font_t* font_create(SDL_Renderer* renderer);
#endif
