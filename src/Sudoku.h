#include <SDL.h>b
#include <SDL_ttf.h>

#ifndef SUDOKU_SUDOKU_H
#define SUDOKU_SUDOKU_H
class Sudoku {
public:
    Sudoku();
    void readScheme();
    void fillCurrentField();
    void draw(SDL_Event, bool*);
private:
    void newGame();
    bool checkUserWin();
    void drawGrid(SDL_Renderer*);
    void drawSelection(SDL_Renderer*);
    void drawInvalidCells(SDL_Renderer*);
    void drawDigit(SDL_Renderer*);
    void handleKey(SDL_Event&, bool*);
};
#endif
