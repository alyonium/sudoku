#include <SDL.h>
#include "Sudoku.h"
#include <string>
#include "Texture.h"
#include "GlobalVariables.h"
#include "Background.h"
#include "Level.h"
#include "Victory.h"

#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H
class Game {
    Level level;
    Sudoku sudoku = Sudoku(font);
    Background background;
    Victory victory;
public:
    void handleEvent(SDL_Event *event, bool *isScene);
};
#endif
