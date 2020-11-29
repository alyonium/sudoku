#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "Sudoku.h"
#include "Texture.h"

SDL_Texture* gTexture;

Texture digitTextures[9];

struct CellData {
    int digit = 0; 	//текущая цифра в клетке, 0 для пустой
    int validateCount = 0; // 0 если решение валидное
};

enum AreaType {
    ROW,
    COLUMN,
    BOX
};

class Area {
private:
    AreaType type;   // тип поля
    int index;         // индексы строки/клетки/столбца
    int counter[9]; // счетчик повторяющихся цифр в окружении

public:
    Area() {};
    ~Area() {};

    void init(AreaType type, int index) {
        this->type = type;
        this->index = index;
    }

    void removeDigit(int &digit, CellData (&field)[9][9], int &x, int &y) {
        counter[digit - 1]--;

        int xi;
        int yi;

        if (counter[digit - 1] >= 1) {
            for (int i = 0; i < 9; i++) {
                switch (type) {

                    case ROW: {
                        xi = i;
                        yi = index;
                        break;
                    }

                    case COLUMN: {
                        xi = index;
                        yi = i;
                        break;
                    }

                    case BOX: {
                        xi = 3 * (index % 3) + (i % 3);
                        yi = 3 * (index / 3) + (i / 3);
                        break;
                    }
                }

                if (xi == x && yi == y) { // empty position
                    field[xi][yi].validateCount -= counter[digit-1];
                } else if (field[xi][yi].digit == digit) {
                    field[xi][yi].validateCount--;
                }
            }
        }
    }
    void addDigit(int &digit, CellData (&field)[9][9], int &x, int &y) {
        counter[digit - 1]++; //увеличиваем счетчик кол-ва цифр в "окружении" введенной цифры

        int xi;
        int yi;

        if (counter[digit - 1] > 1) {

            for (int i = 0; i < 9; i++) {

                switch (type) {
                    case ROW: {
                        xi = i;
                        yi = index;
                        break;
                    }

                    case COLUMN: {
                        xi = index;
                        yi = i;
                        break;
                    }

                    case BOX: {
                        xi = 3 * (index % 3) + (i % 3);
                        yi = 3 * (index / 3) + (i / 3);
                        break;
                    }
                }

                if (field[xi][yi].digit == digit) {
                    if (xi == x && yi == y) {
                        field[xi][yi].validateCount += counter[digit - 1] - 1;
                    } else {
                        field[xi][yi].validateCount++;
                    }
                }
            }
        }
    }
};

CellData field[9][9];

Area areaRows[9];
Area areaCols[9];
Area areaBoxes[9];

int selectedX = 0;
int selectedY = 0;

const int CELL_SIZE = 50;


void drawGrid(SDL_Renderer *gRenderer) {
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderClear(gRenderer);

    int fieldSize = 9 * CELL_SIZE;

    SDL_SetRenderDrawColor(gRenderer, 38, 50, 56, 255);

    for (int i = 0; i < 10; i++) {
        int length = i * CELL_SIZE;
        SDL_RenderDrawLine(gRenderer, 0, length, fieldSize, length);
        SDL_RenderDrawLine(gRenderer, length, 0, length, fieldSize);
        if (i % 3 == 0) { //пересечения жирнее
            SDL_RenderDrawLine(gRenderer, 0, length + 1, fieldSize, length + 1);
            SDL_RenderDrawLine(gRenderer, length + 1, 0, length + 1, fieldSize);
        }
    }
}

void drawSelection(SDL_Renderer *gRenderer) {
    int x = selectedX * CELL_SIZE;
    int y = selectedY * CELL_SIZE;

    SDL_SetRenderDrawColor(gRenderer, 187, 222, 251, 255);

    int startX = selectedX % 3 == 0 ?
                 x + 2:
                 x + 1; //толстые или тонкие линии

    int startY = selectedY % 3 == 0 ?
                 y + 2:
                 y + 1;

    int width = selectedX % 3 == 0 ?
                CELL_SIZE - 2:
                CELL_SIZE - 1;

    int height = selectedY % 3 == 0 ?
                 CELL_SIZE - 2:
                 CELL_SIZE - 1;

    SDL_Rect select = {startX, startY, width, height};
    SDL_RenderFillRect (gRenderer, &select);
}

void drawInvalidCells(SDL_Renderer *gRenderer) {
    SDL_SetRenderDrawColor(gRenderer, 244, 143, 177, 255);

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (field[i][j].validateCount != 0) {
                int x = i * CELL_SIZE;
                int y = j * CELL_SIZE;

                int startX = i % 3 == 0 ?
                             x + 2:
                             x + 1;

                int startY = j % 3 == 0 ?
                             y + 2:
                             y + 1;

                int width = i % 3 == 0 ?
                            CELL_SIZE - 2:
                            CELL_SIZE - 1;

                int height = j % 3 == 0 ?
                             CELL_SIZE - 2:
                             CELL_SIZE - 1;

                SDL_Rect select = {startX, startY, width, height};
                SDL_RenderFillRect (gRenderer, &select);
            }
        }
    }
}

void drawDigit(SDL_Renderer *gRenderer) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (field[i][j].digit != 0) {
                Texture fontTexture = digitTextures[field[i][j].digit - 1];

                int x = i * CELL_SIZE;
                int y = j * CELL_SIZE;

                int startX = (i % 3 == 0 ?
                              x + 2:
                              x + 1) + (CELL_SIZE - fontTexture.width) / 2;

                int startY = (j % 3 == 0 ?
                              y + 4:
                              y + 3) + (CELL_SIZE - fontTexture.height) / 2;

                SDL_Rect digitRect = {startX, startY, fontTexture.width, fontTexture.height};
                SDL_RenderCopy( gRenderer, fontTexture.texture, NULL, &digitRect );
            }
        }
    }
}

void handleKey(SDL_Event &event) {
    if (event.type == SDL_KEYDOWN) {
    int pressedKeyCode = event.key.keysym.sym;

    switch(pressedKeyCode) {
        case SDLK_LEFT: {
            if (selectedX != 0)  {
                selectedX--;
            }
            break;
        }

        case SDLK_RIGHT: {
            if (selectedX != 8) {
                selectedX++;
            }
            break;
        }

        case SDLK_UP: {
            if (selectedY != 0)  {
                selectedY--;
            }
            break;
        }

        case SDLK_DOWN: {
            if (selectedY != 8) {
                selectedY++;
            }
            break;
        }

        case SDLK_1: case SDLK_2: case SDLK_3:
        case SDLK_4: case SDLK_5: case SDLK_6:
        case SDLK_7: case SDLK_8: case SDLK_9: {
            int previous = field[selectedX][selectedY].digit; //текущая цифра в ячейке

            int newDigit = pressedKeyCode - SDLK_1 + 1; //новая цифра

            if (previous == newDigit) { //если дважды ввели одно и то же
                break;
            }

            int box = 3 * (selectedY / 3) + (selectedX / 3); //в какой большой клетке из 9 ячеек сейчас находимся

            if (previous != 0) {  //если предыдущая цифра есть
                areaCols[selectedX].removeDigit(previous, field, selectedX, selectedY);
                areaRows[selectedY].removeDigit(previous, field, selectedX, selectedY);
                areaBoxes[box].removeDigit (previous, field, selectedX, selectedY);
            }

            field[selectedX][selectedY].digit = newDigit;
            areaCols[selectedX].addDigit(newDigit, field, selectedX, selectedY);
            areaRows[selectedY].addDigit(newDigit, field, selectedX, selectedY);
            areaBoxes[box].addDigit (newDigit, field, selectedX, selectedY);

            break;
        }

        case SDLK_BACKSPACE: {
            if (field[selectedX][selectedY].digit == 0) { //если и так пустая
                break;
            }

            field[selectedX][selectedY].digit = 0;

            int box = 3 * (selectedY / 3) + (selectedX / 3);

            areaCols[selectedX].removeDigit(field[selectedX][selectedY].digit, field, selectedX, selectedY);
            areaRows[selectedY].removeDigit(field[selectedX][selectedY].digit, field, selectedX, selectedY);
            areaBoxes[box].removeDigit(field[selectedX][selectedY].digit, field, selectedX, selectedY);

            break;
        }
    }
    }
}

Sudoku::Sudoku(SDL_Renderer *gRenderer, TTF_Font *font) {
    this->gRenderer = gRenderer;
    gTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 9 * CELL_SIZE + 2, 9 * CELL_SIZE + 2); // +2 на бордеры
    SDL_Color color = {30, 50, 56};

    for (int i = 0; i < 9; i++) {
        std::string digitString = std::to_string(i + 1);
        SDL_Surface* textSurface = TTF_RenderText_Blended(font, digitString.c_str(), color);

        //создаем поверхности с цифрами
        SDL_Texture *texture = SDL_CreateTextureFromSurface(gRenderer, textSurface);

        Texture fontTexture;
        fontTexture.texture = texture;
        fontTexture.width = textSurface->w;
        fontTexture.height = textSurface->h;
        digitTextures[i] = fontTexture;

        //освобождаем ненужную поверхность
        SDL_FreeSurface(textSurface);
    }

    for (int i = 0; i < 9; i++) {
        areaCols[i].init(COLUMN, i);
    }

    for (int i = 0; i < 9; i++) {
        areaRows[i].init(ROW, i);
    }

    for (int i = 0; i < 9; i++) {
        areaBoxes[i].init(BOX, i);
    }
}

void Sudoku::draw(SDL_Event event) {
    handleKey(event);
    SDL_SetRenderTarget(gRenderer, gTexture);
    drawGrid(gRenderer);
    drawInvalidCells(gRenderer);
    drawSelection(gRenderer);
    drawDigit(gRenderer);
    SDL_SetRenderTarget(gRenderer, NULL);

    int size = 9 * CELL_SIZE + 4;

    SDL_Rect fieldRect = {(750 - size) / 2, (600 - size) / 2, size, size};
    SDL_RenderCopy(gRenderer, gTexture, NULL, &fieldRect);
}