#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>
#include "Texture.h"
#include "Sudoku.h"
#include "Game.h"
#include "GlobalVariables.h"
#include "Rules.h"
#include "Control.h"

using namespace std;

SDL_Window* mainWindow;

int SPLASH_DELAY = 500;

const int MENU_NUMBERS = 4;

bool initialize() {
    if(SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        cout << SDL_GetError() << endl;
        return false;
    }

    mainWindow = SDL_CreateWindow("SUDOKU!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    if (mainWindow == NULL) {
        cout << SDL_GetError() << endl;
        return false;
    }

    gRenderer = SDL_CreateRenderer(mainWindow, -1, 0);

    if (gRenderer == NULL) {
        cout << SDL_GetError() << endl;
        return false;
    }

    if (TTF_Init () < 0) {
        cout << TTF_GetError() << endl;
        return false;
    }

    font = TTF_OpenFont("font/japanFont.otf", fontSize);
    bigFont = TTF_OpenFont("font/japanFont.otf", bigFontSize);

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        cout << IMG_GetError() << endl;
        return false;
    }

    SDL_SetWindowTitle(mainWindow, "SUDOKU!");

    return true;
}

void renderText(Texture fontTexture, TTF_Font *font, const char *text, SDL_Color color, int xStart, int yStart) {
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
    fontTexture.texture = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_QueryTexture(fontTexture.texture, NULL, NULL, &fontTexture.width, &fontTexture.height);
    SDL_Rect rect = { xStart, yStart, fontTexture.width, fontTexture.height };
    SDL_RenderCopy(gRenderer, fontTexture.texture, NULL, &rect);
}

void splashScreen() {
    Texture splashTexture;
    splashTexture.width = SCREEN_WIDTH;
    splashTexture.height = SCREEN_HEIGHT;
    const char *name = "SUDOKU";

    Texture myName;
    myName.width = 500;
    myName.height = 40;
    const char *by = "made by alyonium";

    SDL_SetRenderDrawColor(gRenderer, 244, 143, 177, 255);
    SDL_RenderClear(gRenderer);

    renderText(splashTexture, bigFont, name, fontColor, (SCREEN_WIDTH  / 3), (SCREEN_HEIGHT  / 2) - (bigFontSize / 2));
    renderText(myName, font, by, fontColor, (SCREEN_WIDTH  / 3) + 20, (SCREEN_HEIGHT - (bigFontSize / 2)));

    SDL_SetRenderTarget(gRenderer, NULL);
    SDL_RenderPresent(gRenderer);
    SDL_Delay(SPLASH_DELAY);
}

void closeEverything() {
    if (gRenderer != NULL)
        SDL_DestroyRenderer(gRenderer);

    if (mainWindow != NULL)
        SDL_DestroyWindow(mainWindow);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int showMenu(SDL_Renderer *renderer) {
    int x;
    int y;
    bool isMenu;
    int width = SCREEN_WIDTH;
    int height = SCREEN_HEIGHT;

    const char *labels[MENU_NUMBERS] = { "Начать игру", "Правила", "Управление", "Выход" };
    SDL_Surface *menuTextTexture[MENU_NUMBERS];

    bool hovered[MENU_NUMBERS] = {false, false, false, false};
    bool selected[MENU_NUMBERS] = {false, false, false, false};

    SDL_Color color[2] = {menuColor, menuColorHover};

    SDL_Rect pos[MENU_NUMBERS];

    Game game;

    for (int i = 0; i < MENU_NUMBERS; ++i) {
        menuTextTexture[i] = TTF_RenderUTF8_Blended(font, labels[i], color[0]);
        pos[i].x = SCREEN_WIDTH / 3;
        pos[i].y = (SCREEN_HEIGHT / 3) + i * 60;
        pos[i].w = menuTextTexture[i]->clip_rect.w;
        pos[i].h = menuTextTexture[i]->clip_rect.h;
    }

    SDL_Event event;
    while (1) {
        switch (event.type) {
            case SDL_QUIT:
                for (int i = 0; i < MENU_NUMBERS; ++i) {
                    SDL_FreeSurface(menuTextTexture[i]);
                }
                closeEverything();
                return 1;
        }

        for (int i = 0; i < MENU_NUMBERS; ++i) {
            if (selected[i] && i == 0) { //запуск игры
                game.handleEvent(&event, &selected[i]);
                break;
            }

            if (selected[i] && i == 1) { //правила
                Rules rules;
                rules.handleEvent(&event, &selected[i]);
                break;
            }

            if (selected[i] && i == 2) { //управление
                Control control;
                control.handleEvent(&event, &selected[i]);
                break;
            }

            if (selected[i] && i == 3) { //выход из игры
                for (int i = 0; i < MENU_NUMBERS; ++i) {
                    SDL_FreeSurface(menuTextTexture[i]);
                }
                closeEverything();
                return 1;
            }
        }

        isMenu = true;

        for(int i = 0; i< MENU_NUMBERS; ++i) {
            if(selected[i]) {
                isMenu = false;
                break;
            }
        }

        SDL_RenderPresent(gRenderer);

        if(!isMenu){
            continue;
        }

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_MOUSEMOTION:
                    x = event.motion.x;
                    y = event.motion.y;
                    for (int i = 0; i < MENU_NUMBERS; i++) {
                        if (x >= pos[i].x && x <= pos[i].x + pos[i].w &&
                            y >= pos[i].y && y <= pos[i].y + pos[i].h) {
                            if (!hovered[i]) {
                                hovered[i] = true;
                                SDL_FreeSurface(menuTextTexture[i]);
                                menuTextTexture[i] = TTF_RenderUTF8_Blended(font, labels[i], color[1]);
                            }
                        } else if (hovered[i]) {
                            hovered[i] = false;
                            SDL_FreeSurface(menuTextTexture[i]);
                            menuTextTexture[i] = TTF_RenderUTF8_Blended(font, labels[i], color[0]);
                        }
                    }
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    x = event.button.x;
                    y = event.button.y;
                    for (int i = 0; i < MENU_NUMBERS; i += 1) {
                        if (x >= pos[i].x && x <= pos[i].x + pos[i].w &&
                            y >= pos[i].y && y <= pos[i].y + pos[i].h) {
                            selected[i] = true;
                            if (i == 0) {
                                step = BACKGROUND;
                            }
                        }
                    }
                    break;

                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        for (int i = 0; i < MENU_NUMBERS; ++i) {
                            SDL_FreeSurface(menuTextTexture[i]);
                        }
                        closeEverything();
                        return 1;
                    }
            }
        }

        SDL_SetRenderDrawColor(renderer, 209, 196, 233, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < MENU_NUMBERS; i++) {
            Texture menuItemTexture;

            menuItemTexture.texture = SDL_CreateTextureFromSurface(renderer, menuTextTexture[i]);
            menuItemTexture.width = width;
            menuItemTexture.height = height;

            SDL_QueryTexture(menuItemTexture.texture, NULL, NULL, &menuItemTexture.width, &menuItemTexture.height);
            SDL_Rect rectGroup = { pos[i].x, pos[i].y, menuItemTexture.width, menuItemTexture.height };
            SDL_RenderCopy(renderer, menuItemTexture.texture, NULL, &rectGroup);
        }
    }
}

int main( int argc, char* args[] ) {

    if (!initialize()) {
        return -1;
    }

    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        splashScreen();
        break;
    }

    showMenu(gRenderer);

    closeEverything();
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();
    return 0;
}