#include "SDL.h"
#include "SDL_ttf.h"
#include "Texture.h"

#ifndef SUDOKU_CONTROL_H
#define SUDOKU_CONTROL_H


class Control {
public:
    Control()=default;
    void handleEvent(SDL_Event* e, bool *isScene);
    void renderText(const char *text, SDL_Color color, int xStart, int yStart, TTF_Font* font);
};

#endif
