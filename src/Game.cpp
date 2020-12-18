#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include "Sudoku.h"
#include "Texture.h"
#include "GlobalVariables.h"

Game::Game() {
    setBackground("img/japan-0.jpg");
}

void Game::setBackground(const std::string &file) {
    SDL_Surface *image;

    image = IMG_Load(file.c_str());

    backgroundTexture.width = image->w;
    backgroundTexture.height = image->h;
    backgroundTexture.texture = SDL_CreateTextureFromSurface(gRenderer, image);
};

void Game::handleEvent(SDL_Event *event, bool *isScene) {
        while (SDL_PollEvent(event) != 0) {
            if (event->type == SDL_QUIT) {
                break;
            }

            if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE) {
                *isScene = false;
                break;
            }

            SDL_RenderClear(gRenderer);

            SDL_Rect rect = {0, 0, backgroundTexture.width, backgroundTexture.height};
            SDL_RenderCopy(gRenderer, backgroundTexture.texture, NULL, &rect);

            sudoku.draw(*event);
        }
}
