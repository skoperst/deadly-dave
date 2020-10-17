#ifndef _FONT_H_
#define _FONT_H_

#include <SDL.h>

/*
 * We have two fonts one for black letters, one for white letters.
 * Yes, its possible to do one font and color it, I don't know how to do it.
 */
typedef struct font_struct {
    SDL_Texture* white[256];
    SDL_Texture* black[256];
} font_t;

font_t* font_create(SDL_Renderer* renderer);
#endif
