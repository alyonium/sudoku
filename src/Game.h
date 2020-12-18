#include <SDL.h>
#include "Sudoku.h"
#include <string>
#include "Texture.h"
#include "GlobalVariables.h"

#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H
class Game {
    Sudoku sudoku = Sudoku(gRenderer, font);
    Texture backgroundTexture;
public:
    Game();
    void setBackground(const std::string &file);
    void handleEvent(SDL_Event *event, bool *isScene);
};
#endif
