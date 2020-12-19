#include "Background.h"
#include "SDL.h"
#include "GlobalVariables.h"
#include "SDL_ttf.h"
#include <string>
#include <SDL_image.h>

void Background::renderText(const char *text, SDL_Color color, int xStart, int yStart, TTF_Font *font) {
    int width = SCREEN_WIDTH, height = SCREEN_HEIGHT;
    SDL_Surface *surfaceGroup =TTF_RenderUTF8_Blended(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(gRenderer, surfaceGroup);
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    SDL_Rect rectGroup = {xStart, yStart, width, height};
    SDL_RenderCopy(gRenderer, texture, NULL, &rectGroup);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surfaceGroup);
}

void Background::handleEvent(SDL_Event *e){
    int width = SCREEN_WIDTH;
    int height = SCREEN_HEIGHT;

    const char *labels[4] = { "img/autumnSmall.jpg", "img/sakuraSmall.jpg", "img/tokyoSmall.jpg", "img/fujiyamaSmall.jpg" };
    SDL_Surface *menuTexture[4];

    bool selected[4] = {false, false, false, false};

    SDL_Rect pos[4];

    for (int i = 0; i < 4; ++i) {
        menuTexture[i] = IMG_Load(labels[i]);

        pos[i].x = SCREEN_WIDTH / 4;
        pos[i].y = (SCREEN_HEIGHT / 4) + i * 60;
        pos[i].w = menuTexture[i]->w;
        pos[i].h = menuTexture[i]->h;

        SDL_CreateTextureFromSurface(gRenderer, menuTexture[i]);
    }


    int x;
    int y;

    switch (e->type) {

        case SDL_MOUSEBUTTONDOWN:

            x = e->button.x;
            y = e->button.y;
            for (int i = 0; i < 4; i += 1) {
                if (x >= pos[i].x && x <= pos[i].x + pos[i].w &&
                    y >= pos[i].y && y <= pos[i].y + pos[i].h) {
                    selected[i] = true;
                    type = SUDOKU;
                    if (i == 0) {
                        pictureName = AUTUMN;
                    } else if(i == 1) {
                        pictureName = SAKURA;
                    } else if(i == 2) {
                        pictureName = TOKYO;
                    } else if(i == 3) {
                        pictureName = FUJIYAMA;
                    }
                }
            }
            break;
    }

    SDL_SetRenderDrawColor(gRenderer, 248, 187, 208, 255);
    SDL_RenderClear(gRenderer);

    for (int i = 0; i < 4; i++) {
        Texture menuItemTexture;

        menuItemTexture.texture = SDL_CreateTextureFromSurface(gRenderer, menuTexture[i]);
        menuItemTexture.width = width;
        menuItemTexture.height = height;

        SDL_QueryTexture(menuItemTexture.texture, NULL, NULL, &menuItemTexture.width, &menuItemTexture.height);
        SDL_Rect rectGroup = { pos[i].x, pos[i].y, menuItemTexture.width, menuItemTexture.height };
        SDL_RenderCopy(gRenderer, menuItemTexture.texture, NULL, &rectGroup);
    }

    renderText("Выберите фон", fontColor, SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 4 - 20, font);

    SDL_RenderPresent(gRenderer);
}
