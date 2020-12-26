#include "Game.h"
#include <SDL.h>
#include "Sudoku.h"
#include "Texture.h"
#include "GlobalVariables.h"
#include <iostream>
#include "Background.h"
#include "Level.h"

Game::Game() {
//    sudoku.readScheme();
}

void Game::handleEvent(SDL_Event *event, bool *isScene) {
        while (SDL_PollEvent(event) != 0) {
            if (event->type == SDL_QUIT) {
                break;
            }

            if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE) {
                *isScene = false;
                break;
            }

            if (step == BACKGROUND) {
                background.handleEvent(event);
            }

            if (step == LEVEL) {
                level.handleEvent(event);
            }

            if (step == SUDOKU) {
                SDL_RenderClear(gRenderer);

                SDL_Rect rect = {0, 0, backgroundTexture.width, backgroundTexture.height};
                SDL_RenderCopy(gRenderer, backgroundTexture.texture, NULL, &rect);

                sudoku.draw(*event);
            }
        }
}
