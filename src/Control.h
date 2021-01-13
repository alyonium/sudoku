#include "SDL.h"
#include "SDL_ttf.h"
#include "Texture.h"
#include "Menu.h"

#ifndef SUDOKU_CONTROL_H
#define SUDOKU_CONTROL_H

class Control : public Menu {
public:
    void handleEvent(SDL_Event* e, bool *isScene) override;
    void renderText(const char *, SDL_Color, int, int, TTF_Font*) override;
};

#endif
