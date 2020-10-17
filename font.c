#include "font.h"

font_t* font_create(SDL_Renderer* renderer) {
    font_t* font = malloc(sizeof(font_t));

    font->white[(int)'A'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/a.bmp"));
    font->black[(int)'A'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/a.bmp"));

    font->white[(int)'B'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/b.bmp"));
    font->black[(int)'B'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/b.bmp"));

    font->white[(int)'C'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/c.bmp"));
    font->black[(int)'C'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/c.bmp"));

    font->white[(int)'D'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/d.bmp"));
    font->black[(int)'D'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/d.bmp"));

    font->white[(int)'E'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/e.bmp"));
    font->black[(int)'E'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/e.bmp"));

    font->white[(int)'F'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/f.bmp"));
    font->black[(int)'F'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/f.bmp"));

    font->white[(int)'G'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/g.bmp"));
    font->black[(int)'G'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/g.bmp"));

    font->white[(int)'H'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/h.bmp"));
    font->black[(int)'H'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/h.bmp"));

    font->white[(int)'I'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/i.bmp"));
    font->black[(int)'I'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/i.bmp"));

    font->white[(int)'J'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/j.bmp"));
    font->black[(int)'J'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/j.bmp"));

    font->white[(int)'K'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/k.bmp"));
    font->black[(int)'K'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/k.bmp"));

    font->white[(int)'L'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/l.bmp"));
    font->black[(int)'L'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/l.bmp"));

    font->white[(int)'M'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/m.bmp"));
    font->black[(int)'M'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/m.bmp"));

    font->white[(int)'N'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/n.bmp"));
    font->black[(int)'N'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/n.bmp"));

    font->white[(int)'O'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/o.bmp"));
    font->black[(int)'O'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/o.bmp"));

    font->white[(int)'P'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/p.bmp"));
    font->black[(int)'P'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/p.bmp"));

    font->white[(int)'Q'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/q.bmp"));
    font->black[(int)'Q'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/q.bmp"));

    font->white[(int)'R'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/r.bmp"));
    font->black[(int)'R'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/r.bmp"));

    font->white[(int)'S'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/s.bmp"));
    font->black[(int)'S'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/s.bmp"));

    font->white[(int)'T'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/t.bmp"));
    font->black[(int)'T'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/t.bmp"));

    font->white[(int)'U'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/u.bmp"));
    font->black[(int)'U'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/u.bmp"));

    font->white[(int)'V'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/v.bmp"));
    font->black[(int)'V'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/v.bmp"));

    font->white[(int)'W'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/w.bmp"));
    font->black[(int)'W'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/w.bmp"));

    font->white[(int)'X'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/x.bmp"));
    font->black[(int)'X'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/x.bmp"));

    font->white[(int)'Y'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/y.bmp"));
    font->black[(int)'Y'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/y.bmp"));

    font->white[(int)'Z'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/z.bmp"));
    font->black[(int)'Z'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/z.bmp"));

    font->white[(int)'0'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/0.bmp"));
    font->black[(int)'0'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/0.bmp"));

    font->white[(int)'1'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/1.bmp"));
    font->black[(int)'1'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/1.bmp"));

    font->white[(int)'2'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/2.bmp"));
    font->black[(int)'2'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/2.bmp"));

    font->white[(int)'3'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/3.bmp"));
    font->black[(int)'3'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/3.bmp"));

    font->white[(int)'4'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/4.bmp"));
    font->black[(int)'4'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/4.bmp"));

    font->white[(int)'5'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/5.bmp"));
    font->black[(int)'5'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/5.bmp"));

    font->white[(int)'6'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/6.bmp"));
    font->black[(int)'6'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/6.bmp"));

    font->white[(int)'7'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/7.bmp"));
    font->black[(int)'7'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/7.bmp"));

    font->white[(int)'8'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/8.bmp"));
    font->black[(int)'8'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/8.bmp"));

    font->white[(int)'9'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/9.bmp"));
    font->black[(int)'9'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/9.bmp"));

    font->white[(int)' '] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/space.bmp"));
    font->black[(int)' '] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/space.bmp"));

    font->white[(int)','] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/comma.bmp"));
    font->black[(int)','] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/comma.bmp"));

    font->white[(int)'.'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/dot.bmp"));
    font->black[(int)'.'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/dot.bmp"));

    font->white[(int)'('] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/pthesis_l.bmp"));
    font->black[(int)'('] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/pthesis_l.bmp"));

    font->white[(int)')'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/pthesis_r.bmp"));
    font->black[(int)')'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/pthesis_r.bmp"));

    font->white[(int)'!'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/exclamation.bmp"));
//    font->black[(int)'!'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/exclamation.bmp"));


    font->black[(int)'?'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/question.bmp"));
    font->black[(int)'!'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/question.bmp"));
    font->black[(int)':'] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("res/font/black/colon.bmp"));

    return font;
}

