#include "SDL_ttf.h"
#include <SDL.h>

#ifndef SUDOKU_MENU_H
#define SUDOKU_MENU_H

class Menu {
public:
    virtual void handleEvent(SDL_Event *e, bool *scene) { };
    virtual void handleEvent(SDL_Event *e) { };
    virtual void renderText(const char *, SDL_Color, int, int, TTF_Font *) = 0;
};

#endif
