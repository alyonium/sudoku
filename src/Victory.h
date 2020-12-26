#include "SDL.h"
#include "SDL_ttf.h"
#include "Texture.h"

#ifndef SUDOKU_VICTORY_H
#define SUDOKU_VICTORY_H

class Victory {
public:
    Victory()=default;
    void handleEvent(SDL_Event* e, bool *scene);
    void renderText(const char *text, SDL_Color color, int xStart, int yStart, TTF_Font* font);
};


#endif
