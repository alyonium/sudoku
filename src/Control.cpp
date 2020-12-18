#include "Control.h"
#include "SDL.h"
#include "GlobalVariables.h"
#include "SDL_ttf.h"
#include <string>

void Control::renderText(const char *text, SDL_Color color, int xStart, int yStart, TTF_Font *font) {
    int width = SCREEN_WIDTH, height = SCREEN_HEIGHT;
    SDL_Surface *surfaceGroup =TTF_RenderUTF8_Blended(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(gRenderer, surfaceGroup);
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    SDL_Rect rectGroup = {xStart, yStart, width, height};
    SDL_RenderCopy(gRenderer, texture, NULL, &rectGroup);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surfaceGroup);
}

void Control::handleEvent(SDL_Event *e, bool *isScene){
    while (SDL_PollEvent(e)) {
        if(e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_ESCAPE){
            *isScene = false;
            break;
        }
    }
    SDL_SetRenderDrawColor(gRenderer, 225, 190, 231, 255);
    SDL_RenderClear(gRenderer);

    renderText("Управление", fontColor, SCREEN_WIDTH / 2 - 90, SCREEN_HEIGHT / 5 - 20, font);

    renderText("Для перемещения по игровому полю используйте", fontColor, 40, SCREEN_HEIGHT / 5 + 50,  font);

    renderText("клавиши со стрелками. Для заполнения ячеек", fontColor, 60, SCREEN_HEIGHT / 5 + 100, font);

    renderText("используйте клавиши с цифрами. Для удаления цифры", fontColor, 20, SCREEN_HEIGHT / 5 + 150, font);

    renderText("из ячейки используйте клавишу delete. Если вы", fontColor, 40, SCREEN_HEIGHT / 5 + 200, font);

    renderText("введете цифру, не соответствующую решению, она", fontColor, 30, SCREEN_HEIGHT / 5 + 250, font);

    renderText("подсветится красным.", fontColor, 220, SCREEN_HEIGHT / 5 + 300, font);

    SDL_RenderPresent(gRenderer);
}
