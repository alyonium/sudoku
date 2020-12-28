#include "GlobalVariables.h"
#include "SDL_image.h"
#include "Texture.h"

int SCREEN_WIDTH = 750;
int SCREEN_HEIGHT = 600;

SDL_Renderer *gRenderer = NULL;

SDL_Color fontColor = {30, 50, 56};
SDL_Color menuColorHover = {69,   60,   100};
SDL_Color menuColor = {255, 255, 255};

TTF_Font* font;
TTF_Font* bigFont;

int fontSize = 35;
int bigFontSize = 100;

StepType step = BACKGROUND;

LevelType levelDifficulty = EASY;

Texture backgroundTexture;

void setBackground(char *name) {
    SDL_Surface *image;
    const char* pictureName = name;
    image = IMG_Load(pictureName);
    backgroundTexture.width = image->w;
    backgroundTexture.height = image->h;
    backgroundTexture.texture = SDL_CreateTextureFromSurface(gRenderer, image);
};

CellData initialField[9][9];
CellData currentField[9][9];

char *filename = "schemes/low.txt";

SDL_Rect backButtonRect = { 10, 10, 30, 30 };
SDL_Texture *backButtonTexture;

void backButton() {
    int width = SCREEN_WIDTH;
    int height = SCREEN_HEIGHT;
    SDL_Surface *backButton;
    backButton = SDL_CreateRGBSurface(0, 30, 30, 32, 0, 0, 0, 0);
    SDL_FillRect(backButton, NULL, SDL_MapRGB(backButton->format, 255, 0, 0));

    backButtonTexture = SDL_CreateTextureFromSurface(gRenderer, backButton);
    SDL_QueryTexture(backButtonTexture, NULL, NULL, &width, &height);
    SDL_RenderCopy(gRenderer, backButtonTexture, NULL, &backButtonRect);
}

bool addBackButton = false;