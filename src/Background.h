//#include "Texture.h"
//#include "GlobalVariables.h"
//
//#ifndef SUDOKU_BACKGROUND_H
//#define SUDOKU_BACKGROUND_H
//
//class Background {
//    SDL_Renderer *gRenderer;
//    int x = 0;
//    int y = 0;
//    static const int NUMMENU = 4;
//    int width = SCREEN_WIDTH;
//    int height = SCREEN_HEIGHT;
//
//    const char *labels[NUMMENU] = {"Осень", "Сакура", "Старый Токио", "Фудзияма"};
//    bool hovered[NUMMENU] = {false, false, false, false};
//    SDL_Color color[2] = {menuColor, menuColorHover};
//    SDL_Surface *menus[NUMMENU]{};
//    SDL_Rect pos[NUMMENU]{};
//
//public:
//    Background(SDL_Renderer *gRenderer);
//    void changeBackground();
//    void setBackground(Texture *backgroundTexture);
//};
//#endif