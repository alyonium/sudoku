#include "SDL.h"
#include "SDL_ttf.h"
#include "Texture.h"
#include "Level.h"
#include "GlobalVariables.h"
#include <string>
#include <iostream>
#include "Sudoku.h"

void Level::renderText(const char *text, SDL_Color color, int xStart, int yStart, TTF_Font *font) {
    int width = SCREEN_WIDTH, height = SCREEN_HEIGHT;
    SDL_Surface *surfaceGroup =TTF_RenderUTF8_Blended(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(gRenderer, surfaceGroup);
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    SDL_Rect rectGroup = {xStart, yStart, width, height};
    SDL_RenderCopy(gRenderer, texture, NULL, &rectGroup);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surfaceGroup);
}

void Level::handleEvent(SDL_Event *e, bool *isScene){
    int width = SCREEN_WIDTH;
    int height = SCREEN_HEIGHT;

    const char *labels[3] = { "Ученик", "Боец", "Сенсеи"};
    SDL_Surface *menuTexture[3];

    bool hovered[3] = {false, false, false};
    bool selected[3] = {false, false, false};

    SDL_Color color[2] = {menuColor, menuColorHover};

    SDL_Rect pos[3];

    for (int i = 0; i < 3; ++i) {
        menuTexture[i] = TTF_RenderUTF8_Blended(font, labels[i], color[0]);

        pos[i].x = (SCREEN_WIDTH / 3) + (i == 1? 75 : 60);
        pos[i].y = (SCREEN_HEIGHT / 3 + 20) + i * 80;
        pos[i].w = menuTexture[i]->clip_rect.w;
        pos[i].h = menuTexture[i]->clip_rect.h;
    }

    int x;
    int y;

    switch (e->type) {

        case SDL_MOUSEMOTION:
            x = e->motion.x;
            y = e->motion.y;
            for (int i = 0; i < 3; i++) {
                if (x >= pos[i].x && x <= pos[i].x + pos[i].w &&
                    y >= pos[i].y && y <= pos[i].y + pos[i].h) {
                    if (!hovered[i]) {
                        hovered[i] = true;
                        SDL_FreeSurface(menuTexture[i]);
                        menuTexture[i] = TTF_RenderUTF8_Blended(font, labels[i], color[1]);
                    }
                } else if (hovered[i]) {
                    hovered[i] = false;
                    SDL_FreeSurface(menuTexture[i]);
                    menuTexture[i] = TTF_RenderUTF8_Blended(font, labels[i], color[0]);
                }
            }
            break;

        case SDL_MOUSEBUTTONDOWN:

            x = e->button.x;
            y = e->button.y;
            for (int i = 0; i < 3; i += 1) {
                if (x >= pos[i].x && x <= pos[i].x + pos[i].w &&
                    y >= pos[i].y && y <= pos[i].y + pos[i].h) {
                    selected[i] = true;
                    if (i == 0) {
                        filename = "schemes/low.txt";
                        levelDifficulty = EASY;
                    } else if(i == 1) {
                        filename = "schemes/middle.txt";
                        levelDifficulty = MIDDLE;
                    } else if(i == 2) {
                        filename = "schemes/hard.txt";
                        levelDifficulty = HARD;
                    }
                    step = SUDOKU;
                }
            }


            if (x >= backButtonRect.x && x <= backButtonRect.x + backButtonRect.w &&
                y >= backButtonRect.y && y <= backButtonRect.y + backButtonRect.h) {
                *isScene = false;
            }

            break;
    }

    SDL_SetRenderDrawColor(gRenderer, 248, 187, 208, 255);
    SDL_RenderClear(gRenderer);

    for (int i = 0; i < 3; i++) {
        Texture menuItemTexture;

        menuItemTexture.texture = SDL_CreateTextureFromSurface(gRenderer, menuTexture[i]);
        menuItemTexture.width = width;
        menuItemTexture.height = height;

        SDL_QueryTexture(menuItemTexture.texture, NULL, NULL, &menuItemTexture.width, &menuItemTexture.height);
        SDL_Rect rectGroup = { pos[i].x, pos[i].y, menuItemTexture.width, menuItemTexture.height };
        SDL_RenderCopy(gRenderer, menuItemTexture.texture, NULL, &rectGroup);
    }

    backButton();

    renderText("Выберите сложность", fontColor, SCREEN_WIDTH / 2 - 150, (SCREEN_HEIGHT / 3) - 50, font);

    SDL_RenderPresent(gRenderer);
}

