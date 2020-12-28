#include "SDL.h"
#include "SDL_ttf.h"
#include "Texture.h"

#ifndef SUDOKU_LEVEL_H
#define SUDOKU_LEVEL_H


class Level {
public:
    void handleEvent(SDL_Event* e);
    void renderText(const char *text, SDL_Color color, int xStart, int yStart, TTF_Font* font);
};


#endif
