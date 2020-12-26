#include "Victory.h"
#include "SDL.h"
#include "GlobalVariables.h"
#include "SDL_ttf.h"
#include <string>

void Victory::renderText(const char *text, SDL_Color color, int xStart, int yStart, TTF_Font *font) {
    int width = SCREEN_WIDTH, height = SCREEN_HEIGHT;
    SDL_Surface *surfaceGroup = TTF_RenderUTF8_Blended(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(gRenderer, surfaceGroup);
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    SDL_Rect rectGroup = {xStart, yStart, width, height};
    SDL_RenderCopy(gRenderer, texture, NULL, &rectGroup);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surfaceGroup);
}

void Victory::handleEvent(SDL_Event *e, bool *scene) {
    int width = SCREEN_WIDTH;
    int height = SCREEN_HEIGHT;

    const char *label = "Вернуться в главное меню";
    SDL_Surface *menuTexture;

    bool hovered = false;

    SDL_Color color[2] = {menuColor, menuColorHover};

    SDL_Rect pos;

    menuTexture = TTF_RenderUTF8_Blended(font, label, color[0]);
    pos.x = (SCREEN_WIDTH / 2) - 160;
    pos.y = (SCREEN_HEIGHT / 3) + 70;
    pos.w = menuTexture->w;
    pos.h = menuTexture->h;

    int x;
    int y;

    switch (e->type) {

        case SDL_MOUSEMOTION:
            x = e->button.x;
            y = e->button.y;
            if (x >= pos.x && x <= pos.x + pos.w &&
                y >= pos.y && y <= pos.y + pos.h) {
                if (!hovered) {
                    hovered = true;
                    SDL_FreeSurface(menuTexture);
                    menuTexture = TTF_RenderUTF8_Blended(font, label, color[1]);
                }
            }
            break;


        case SDL_MOUSEBUTTONUP:
            x = e->button.x;
            y = e->button.y;
            if (x >= pos.x && x <= pos.x + pos.w &&
                y >= pos.y && y <= pos.y + pos.h) {
                menuTexture = TTF_RenderUTF8_Blended(font, label, color[1]);
                *scene = false;
            }
            break;
    }

    SDL_SetRenderDrawColor(gRenderer, 179, 157, 219, 255);
    SDL_RenderClear(gRenderer);

    Texture menuItemTexture;

    menuItemTexture.texture = SDL_CreateTextureFromSurface(gRenderer, menuTexture);
    menuItemTexture.width = width;
    menuItemTexture.height = height;

    SDL_QueryTexture(menuItemTexture.texture, NULL, NULL, &menuItemTexture.width, &menuItemTexture.height);
    SDL_Rect rectGroup = {pos.x, pos.y, menuItemTexture.width, menuItemTexture.height};
    SDL_RenderCopy(gRenderer, menuItemTexture.texture, NULL, &rectGroup);

    renderText("Вы победили!", fontColor, SCREEN_WIDTH / 2 - 80, (SCREEN_HEIGHT / 3) , font);

    SDL_RenderPresent(gRenderer);
}
