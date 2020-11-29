#include <SDL.h>
#include "Sudoku.h"
#include <string>
#include "Texture.h"

#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H
class Game {
    SDL_Renderer *gRenderer;
public:
    Game(SDL_Renderer *gRenderer);
    void setBackground(const std::string &file, Texture *backgroundTexture);
    void handleEvent(SDL_Event *event, bool *isScene, TTF_Font *font);
};
#endif
