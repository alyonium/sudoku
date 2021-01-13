#include "SDL.h"
#include "SDL_ttf.h"
#include "Texture.h"
#include "Menu.h"

#ifndef SUDOKU_LEVEL_H
#define SUDOKU_LEVEL_H

class Level : public Menu{
public:
    void handleEvent(SDL_Event* e) override;
    void renderText(const char *, SDL_Color, int, int, TTF_Font*) override;
};


#endif
