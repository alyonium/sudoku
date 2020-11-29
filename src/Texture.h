#include <SDL.h>
#ifndef SUDOKU_TEXTURE_H
#define SUDOKU_TEXTURE_H
struct Texture {
    SDL_Texture* texture;
    int width;
    int height;
};
#endif
