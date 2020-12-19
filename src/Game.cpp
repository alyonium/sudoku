#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include "Sudoku.h"
#include "Texture.h"
#include "GlobalVariables.h"
#include <iostream>
#include "Background.h"

Game::Game() {
    sudoku.readScheme();
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

            if (type == BACKGROUND) {
                Background background;
                background.handleEvent(event);

                if (pictureName == TOKYO) {
                    setBackground("img/tokyo.jpg");
                } else if (pictureName == AUTUMN) {
                    setBackground("img/autumn.jpg");
                } else if (pictureName == SAKURA) {
                    setBackground("img/sakura.jpg");
                } else if (pictureName == FUJIYAMA) {
                    setBackground("img/fujiyama.jpg");
                }

            }

            if (type == LEVEL) {
                std::cout << "2" << std::endl;
            }

            if (type == SUDOKU) {
                SDL_RenderClear(gRenderer);

                SDL_Rect rect = {0, 0, backgroundTexture.width, backgroundTexture.height};
                SDL_RenderCopy(gRenderer, backgroundTexture.texture, NULL, &rect);

                sudoku.draw(*event);
            }
        }
}
