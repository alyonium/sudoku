#include "SDL.h"
#include "SDL_ttf.h"
#include "Texture.h"
#include "Menu.h"

#ifndef SUDOKU_VICTORY_H
#define SUDOKU_VICTORY_H

class Victory : public Menu {
public:
    void handleEvent(SDL_Event* e, bool *scene) override;
    void renderText(const char *, SDL_Color, int, int, TTF_Font*) override;
};


#endif
