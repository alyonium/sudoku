#include <SDL.h>
#include "Sudoku.h"
#include <string>
#include "Texture.h"
#include "GlobalVariables.h"
#include "Background.h"

#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H
class Game {
    Sudoku sudoku = Sudoku(gRenderer, font);
    Background background;
public:
    Game();
    void handleEvent(SDL_Event *event, bool *isScene);
};
#endif
