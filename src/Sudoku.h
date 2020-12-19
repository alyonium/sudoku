#include <SDL.h>
#include <SDL_ttf.h>
#ifndef SUDOKU_SUDOKU_H
#define SUDOKU_SUDOKU_H
class Sudoku {
    SDL_Renderer *gRenderer;
    int fullField[9][9];
    bool noEdit;
public:
    Sudoku(SDL_Renderer*, TTF_Font*);
    void draw(SDL_Event);
    void readScheme();
};
#endif
