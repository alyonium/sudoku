#include "SDL.h"
#include "SDL_ttf.h"
#include "Texture.h"
#include "Menu.h"

#ifndef SUDOKU_BACKGROUND_H
#define SUDOKU_BACKGROUND_H

class Background : public Menu {
public:
    void handleEvent(SDL_Event*, bool *scene) override;
    void renderText(const char*, SDL_Color, int, int, TTF_Font*) override;
    void setBackground(char*);
};

#endif
