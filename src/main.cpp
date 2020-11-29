#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>
#include "Texture.h"
#include "Sudoku.h"

using namespace std;

const int SCREEN_WIDTH = 750;
const int SCREEN_HEIGHT = 600;
int SPLASH_DELAY = 1000;

//Главное окно
SDL_Window* mainWindow;

//Поверхности, накладываемые на главное окно
SDL_Renderer* gRenderer;
SDL_Color fontColor = {30, 50, 56};
TTF_Font* font;
int fontSize = 35;
TTF_Font* bigFont;
int bigFontSize = 100;

Texture backgroundTexture;

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

    SDL_SetRenderDrawColor(gRenderer, 244, 143, 177, 255);
    SDL_RenderClear(gRenderer);

    renderText(splashTexture, bigFont, name, fontColor, (SCREEN_WIDTH  / 3), (SCREEN_HEIGHT  / 2) - (bigFontSize / 2));

    SDL_SetRenderTarget(gRenderer, NULL);
    SDL_RenderPresent(gRenderer);
    SDL_Delay(SPLASH_DELAY);
}

int setBackground(const string &file) {
    SDL_Surface *image;

    image = IMG_Load(file.c_str());

    if (image == NULL) {
        cout << IMG_GetError() << endl;
        return -1;
    }

    backgroundTexture.width = image->w;
    backgroundTexture.height = image->h;
    backgroundTexture.texture = SDL_CreateTextureFromSurface(gRenderer, image);
}

void closeEverything() {
    if (gRenderer != NULL)
        SDL_DestroyRenderer(gRenderer);

    if (mainWindow != NULL)
        SDL_DestroyWindow(mainWindow);

    if (backgroundTexture.texture != NULL)
        SDL_DestroyTexture(backgroundTexture.texture);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main( int argc, char* args[] ) {

    if (!initialize()) {
        return -1;
    }

    SDL_SetWindowTitle(mainWindow, "SUDOKU!");
    Sudoku sudoku(gRenderer, font);

    bool quit = false;
    SDL_Event event;
    bool splash = true;

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            setBackground("img/japan.png");

            while(splash) {
                splashScreen();
                splash = !splash;
            }

            SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
            SDL_RenderClear(gRenderer);

            SDL_Rect rect = {0, 0, backgroundTexture.width, backgroundTexture.height};
            SDL_RenderCopy (gRenderer, backgroundTexture.texture, NULL, &rect);

            sudoku.draw(event);

            SDL_SetRenderTarget(gRenderer, NULL);
            SDL_RenderPresent(gRenderer);

            SDL_RenderPresent(gRenderer);
        }
    }

    closeEverything();
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();
    return 0;
}