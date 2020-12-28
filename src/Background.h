#include "SDL.h"
#include "SDL_ttf.h"
#include "Texture.h"

#ifndef SUDOKU_BACKGROUND_H
#define SUDOKU_BACKGROUND_H


class Background {
public:
    void handleEvent(SDL_Event*, bool*);
    void renderText(const char*, SDL_Color, int, int, TTF_Font*);
};

#endif
