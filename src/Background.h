#include "SDL.h"
#include "SDL_ttf.h"
#include "Texture.h"

#ifndef SUDOKU_BACKGROUND_H
#define SUDOKU_BACKGROUND_H


class Background {
public:
    Background()=default;
    void handleEvent(SDL_Event* e);
    void renderText(const char *text, SDL_Color color, int xStart, int yStart, TTF_Font* font);
};

#endif
