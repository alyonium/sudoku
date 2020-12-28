#include <SDL.h>
#include <SDL_ttf.h>

#ifndef SUDOKU_SUDOKU_H
#define SUDOKU_SUDOKU_H
class Sudoku {
public:
    Sudoku(TTF_Font*);
    void draw(SDL_Event, bool*);
    void readScheme();
    void fillCurrentField();
};
#endif
