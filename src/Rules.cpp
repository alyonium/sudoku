#include "Rules.h"
#include "SDL.h"
#include "GlobalVariables.h"
#include "SDL_ttf.h"
#include <string>

void Rules::renderText(const char *text, SDL_Color color, int xStart, int yStart, TTF_Font *font) {
    int width = SCREEN_WIDTH, height = SCREEN_HEIGHT;
    SDL_Surface *surfaceGroup =TTF_RenderUTF8_Blended(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(gRenderer, surfaceGroup);
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    SDL_Rect rectGroup = {xStart, yStart, width, height};
    SDL_RenderCopy(gRenderer, texture, NULL, &rectGroup);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surfaceGroup);
}

void Rules::handleEvent(SDL_Event *e, bool *isScene){
    while (SDL_PollEvent(e)) {
        if(e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_ESCAPE){
            *isScene = false;
            break;
        }
    }
    SDL_SetRenderDrawColor(gRenderer, 248, 187, 208, 255);
    SDL_RenderClear(gRenderer);

    renderText("Правила", fontColor, SCREEN_WIDTH / 2 - 55, SCREEN_HEIGHT / 4 - 20, font);

    renderText("Судоку — это игра-головоломка, где необходимо", fontColor, 40, SCREEN_HEIGHT / 4 + 50,  font);

    renderText("заполнить пустые клетки так, чтобы каждая строка,", fontColor, 20, SCREEN_HEIGHT / 4 + 100, font);

    renderText("каждый столбец, каждый малый квадрат", fontColor, 85, SCREEN_HEIGHT / 4 + 150, font);

    renderText("содержали все цифры от 1 до 9. При этом каждая", fontColor, 30, SCREEN_HEIGHT / 4 + 200, font);

    renderText("цифра встречается только один раз.", fontColor, 110, SCREEN_HEIGHT / 4 + 250, font);

    SDL_RenderPresent(gRenderer);
}
