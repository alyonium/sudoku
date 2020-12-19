#include "SDL.h"
#include "SDL_ttf.h"

#ifndef SUDOKU_GLOBALVARIABLES_H
#define SUDOKU_GLOBALVARIABLES_H

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

extern SDL_Renderer* gRenderer;

extern SDL_Color fontColor;
extern SDL_Color menuColor;
extern SDL_Color menuColorHover;

extern TTF_Font* font;
extern TTF_Font* bigFont;

extern int fontSize;
extern int bigFontSize;

extern enum StepType {
    BACKGROUND,
    LEVEL,
    SUDOKU
};

extern StepType type;

extern enum PictureNames {
    AUTUMN,
    FUJIYAMA,
    TOKYO,
    SAKURA,
};

extern PictureNames pictureName;

#endif
