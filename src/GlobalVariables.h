#include "SDL.h"
#include "SDL_ttf.h"
#include "Texture.h"

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
    SUDOKU,
    VICTORY
};

extern StepType step;

extern enum LevelType {
    EASY,
    MIDDLE,
    HARD
};

extern LevelType levelDifficulty;

extern Texture backgroundTexture;

void setBackground(char *name);

struct CellData {
    int digit = 0; 	//текущая цифра в клетке, 0 для пустой
    int validateCount = 0; // 0 если решение валидное
    bool noEdit = false; //считана со схемы - true, введена юзером - false
};

extern CellData initialField[9][9];
extern CellData currentField[9][9];

#endif
