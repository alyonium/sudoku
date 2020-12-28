#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "Sudoku.h"
#include "Texture.h"
#include <iostream>
#include <fstream>
#include <GlobalVariables.h>

SDL_Texture *gTexture;

Texture digitTextures[9];

enum AreaType {
    ROW,
    COLUMN,
    BOX
};

class Area {
private:
    AreaType type;   // тип поля
    int index;      // индексы строки/клетки/столбца
    int counter[9] = {0};// счетчик конкретных цифр в строке/клетке/столбце

public:
    Area() {};

    ~Area() {};

    void init(AreaType type, int index) {
        this->type = type;
        this->index = index;
    }

    void removeDigit(int &digit, int &x, int &y) {

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

                if (xi == x && yi == y) {
                    currentField[yi][xi].validateCount -= counter[digit - 1];
                } else if (currentField[yi][xi].digit == digit) {
                    currentField[yi][xi].validateCount--;
                }
            }
        }
    }

    void addDigit(int &digit, int &x, int &y) {

        counter[digit - 1]++; //увеличиваем счетчик кол-ва одинаковых цифр в "окружении" введенной цифры

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

                //если на площади уже есть цифра с таким значением
                if (currentField[yi][xi].digit == digit) {
                    if (xi == x && yi == y) { //если это та цифра, которую мы и ввели
                        currentField[yi][xi].validateCount += counter[digit - 1] - 1;
                    } else {
                        currentField[yi][xi].validateCount++;
                    }
                }
            }
        }
    }

    void cleanCounter() {
        for (int i = 0; i < 9; i++) {
            counter[i] = 0;
        }
    }
};

Area areaRows[9];
Area areaCols[9];
Area areaBoxes[9];

int selectedCol = 0;
int selectedRow = 0;

const int CELL_SIZE = 50;

void newGame() {
    selectedCol = 0;
    selectedRow = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            initialField[i][j].digit = 0;
            initialField[i][j].validateCount = 0;
            initialField[i][j].noEdit = false;

            currentField[i][j].digit = 0;
            currentField[i][j].validateCount = 0;
            currentField[i][j].noEdit = false;
        }
        areaRows[i].cleanCounter();
        areaCols[i].cleanCounter();
        areaBoxes[i].cleanCounter();
    }
}

bool checkUserWin() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (currentField[i][j].digit == 0) {
                return false;
            }
        }
    }
    return true;
}

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
    int horizontalShift = selectedCol * CELL_SIZE;
    int verticalShift = selectedRow * CELL_SIZE;

    SDL_SetRenderDrawColor(gRenderer, 187, 222, 251, 255);

    int startHorizontal = selectedCol % 3 == 0 ?
                          horizontalShift + 2 :
                          horizontalShift + 1; //толстые или тонкие линии

    int startVertical = selectedRow % 3 == 0 ?
                        verticalShift + 2 :
                        verticalShift + 1;

    int width = selectedCol % 3 == 0 ?
                CELL_SIZE - 2 :
                CELL_SIZE - 1;

    int height = selectedRow % 3 == 0 ?
                 CELL_SIZE - 2 :
                 CELL_SIZE - 1;

    SDL_Rect select = {startHorizontal, startVertical, width, height};
    SDL_RenderFillRect(gRenderer, &select);
}

void drawInvalidCells(SDL_Renderer *gRenderer) {
    SDL_SetRenderDrawColor(gRenderer, 244, 143, 177, 255);

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (currentField[i][j].validateCount != 0) {
                int horizontalShift = j * CELL_SIZE;
                int verticalShift = i * CELL_SIZE;

                int startHorizontal = j % 3 == 0 ?
                                      horizontalShift + 2 :
                                      horizontalShift + 1;

                int startVertical = i % 3 == 0 ?
                                    verticalShift + 2 :
                                    verticalShift + 1;

                int width = j % 3 == 0 ?
                            CELL_SIZE - 2 :
                            CELL_SIZE - 1;

                int height = i % 3 == 0 ?
                             CELL_SIZE - 2 :
                             CELL_SIZE - 1;

                SDL_Rect select = {startHorizontal, startVertical, width, height};
                SDL_RenderFillRect(gRenderer, &select);
            }
        }
    }
}

void drawDigit(SDL_Renderer *gRenderer) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {

            if (currentField[i][j].digit != 0) {
                Texture fontTexture = digitTextures[currentField[i][j].digit - 1];

                int verticalShift = i * CELL_SIZE;
                int horizontalShift = j * CELL_SIZE;

                int startVertical = (i % 3 == 0 ?
                                     verticalShift + 2 :
                                     verticalShift + 1) + (CELL_SIZE - fontTexture.height) / 2;

                int startHorizontal = (j % 3 == 0 ?
                                       horizontalShift + 4 :
                                       horizontalShift + 3) + (CELL_SIZE - fontTexture.width) / 2;

                SDL_Rect digitRect = {startHorizontal, startVertical, fontTexture.width, fontTexture.height};
                SDL_RenderCopy(gRenderer, fontTexture.texture, NULL, &digitRect);
            }
        }
    }
}

void handleKey(SDL_Event &event, bool *isScene) {

    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int x = event.button.x;
        int y = event.button.y;

        if (x >= backButtonRect.x && x <= backButtonRect.x + backButtonRect.w &&
            y >= backButtonRect.y && y <= backButtonRect.y + backButtonRect.h) {
            newGame();
            *isScene = false;
        }
    }

    if (event.type == SDL_KEYDOWN) {
        int pressedKeyCode = event.key.keysym.sym;

        switch (pressedKeyCode) {
            case SDLK_LEFT: {
                if (selectedCol != 0) {
                    selectedCol--;
                }
                break;
            }

            case SDLK_RIGHT: {
                if (selectedCol != 8) {
                    selectedCol++;
                }
                break;
            }

            case SDLK_UP: {
                if (selectedRow != 0) {
                    selectedRow--;
                }
                break;
            }

            case SDLK_DOWN: {
                if (selectedRow != 8) {
                    selectedRow++;
                }
                break;
            }

            case SDLK_1:
            case SDLK_2:
            case SDLK_3:
            case SDLK_4:
            case SDLK_5:
            case SDLK_6:
            case SDLK_7:
            case SDLK_8:
            case SDLK_9: {
                int previous = currentField[selectedRow][selectedCol].digit; //текущая цифра в ячейке

                int newDigit = pressedKeyCode - SDLK_1 + 1; //новая цифра

                if (previous == newDigit) { //если дважды ввели одно и то же
                    break;
                }

                int box = 3 * (selectedRow / 3) + (selectedCol / 3); //в какой большой клетке из 9

                if (previous != 0) {  //если предыдущая цифра есть
                    if (currentField[selectedRow][selectedCol].noEdit) {
                        break;
                    } else {
                        areaRows[selectedRow].removeDigit(previous, selectedCol, selectedRow);
                        areaCols[selectedCol].removeDigit(previous, selectedCol, selectedRow);
                        areaBoxes[box].removeDigit(previous, selectedCol, selectedRow);
                    }
                }

                currentField[selectedRow][selectedCol].digit = newDigit;

                areaRows[selectedRow].addDigit(newDigit, selectedCol, selectedRow);
                areaCols[selectedCol].addDigit(newDigit, selectedCol, selectedRow);
                areaBoxes[box].addDigit(newDigit, selectedCol, selectedRow);

                if (checkUserWin()) {
                    step = VICTORY;
                    newGame();
                }

                break;
            }

            case SDLK_BACKSPACE: {
                if (currentField[selectedRow][selectedCol].digit == 0) { //если и так пустая
                    break;
                }

                if (currentField[selectedRow][selectedCol].noEdit) {
                    break;
                }

                int box = 3 * (selectedRow / 3) + (selectedCol / 3);

                areaRows[selectedRow].removeDigit(currentField[selectedRow][selectedCol].digit, selectedCol,
                                                  selectedRow);
                areaCols[selectedCol].removeDigit(currentField[selectedRow][selectedCol].digit, selectedCol,
                                                  selectedRow);
                areaBoxes[box].removeDigit(currentField[selectedRow][selectedCol].digit, selectedCol, selectedRow);

                currentField[selectedRow][selectedCol].digit = 0;

                break;
            }
        }
    }
}

Sudoku::Sudoku(TTF_Font *font) {
    gTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 9 * CELL_SIZE + 2,
                                 9 * CELL_SIZE + 2); // +2 на бордеры
    SDL_Color color = {30, 50, 56};

    for (int i = 0; i < 9; i++) {
        std::string digitString = std::to_string(i + 1);
        SDL_Surface *textSurface = TTF_RenderText_Blended(font, digitString.c_str(), color);

        //создаем поверхности с цифрами
        SDL_Texture *texture = SDL_CreateTextureFromSurface(gRenderer, textSurface);

        Texture fontTexture;
        fontTexture.texture = texture;
        fontTexture.width = textSurface->w;
        fontTexture.height = textSurface->h;
        digitTextures[i] = fontTexture;

        //освобождаем ненужную поверхность
        SDL_FreeSurface(textSurface);

        areaRows[i].init(ROW, i);
        areaCols[i].init(COLUMN, i);
        areaBoxes[i].init(BOX, i);

    }
}

void Sudoku::draw(SDL_Event event, bool *isScene) {
    handleKey(event, isScene);
    SDL_SetRenderTarget(gRenderer, gTexture);
    drawGrid(gRenderer);
    drawSelection(gRenderer);
    drawInvalidCells(gRenderer);
    drawDigit(gRenderer);

    SDL_SetRenderTarget(gRenderer, NULL);

    int size = 9 * CELL_SIZE + 4;

    SDL_Rect fieldRect = {(SCREEN_WIDTH - size) / 2, (SCREEN_HEIGHT - size) / 2, size, size};
    backButton();
    SDL_RenderCopy(gRenderer, gTexture, NULL, &fieldRect);
 }

void Sudoku::readScheme() {
    std::ifstream in(filename);

    if (in.is_open()) {
        in.seekg(0, std::ios::beg);
        in.clear();

        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                in >> initialField[i][j].digit;
            }
        }

        in.close();
    } else {
        std::cout << "File doesn't exist";
    }
}

void Sudoku::fillCurrentField() {

    switch(levelDifficulty) {
        case LOW: {
            for (int i = 0; i < 9; i++) {

                int first = (rand() % 9) + 1;
                int second = (rand() % 8) + 2;
                int third = (rand() % 7) + 3;

                for (int j = 0; j < 9; j++) {
                    if (initialField[i][j].digit != first
                        && initialField[i][j].digit != second
                        && initialField[i][j].digit != third) {

                        currentField[i][j].digit = initialField[i][j].digit;
                        currentField[i][j].noEdit = true;

                        int box = 3 * (i / 3) + (j / 3);

                        areaRows[i].addDigit(currentField[i][j].digit, j, i);
                        areaCols[j].addDigit(currentField[i][j].digit, j, i);
                        areaBoxes[box].addDigit(currentField[i][j].digit, j, i);
                    }
                }
            }
            break;
        }

        case MIDDLE: {
            for (int i = 0; i < 9; i++) {

                int first = (rand() % 9) + 1;
                int second = (rand() % 8) + 2;
                int third = (rand() % 7) + 3;
                int fourth = (rand() % 6) + 4;
                int fifth = (rand() % 5) + 5;

                for (int j = 0; j < 9; j++) {
                    if (initialField[i][j].digit != first
                        && initialField[i][j].digit != second
                        && initialField[i][j].digit != third
                        && initialField[i][j].digit != fourth
                        && initialField[i][j].digit != fifth) {

                        currentField[i][j].digit = initialField[i][j].digit;
                        currentField[i][j].noEdit = true;

                        int box = 3 * (i / 3) + (j / 3);

                        areaRows[i].addDigit(currentField[i][j].digit, j, i);
                        areaCols[j].addDigit(currentField[i][j].digit, j, i);
                        areaBoxes[box].addDigit(currentField[i][j].digit, j, i);
                    }
                }
            }
            break;
        }

        case HARD: {
            for (int i = 0; i < 9; i++) {

                int first = (rand() % 9) + 1;
                int second = (rand() % 8) + 2;
                int third = (rand() % 7) + 3;
                int fourth = (rand() % 6) + 4;
                int fifth = (rand() % 5) + 5;
                int sixth = (rand() % 4) + 6;
                int seventh = (rand() % 3) + 7;
                int eighth = (rand() % 2) + 8;

                for (int j = 0; j < 9; j++) {
                    if (initialField[i][j].digit != first
                        && initialField[i][j].digit != second
                        && initialField[i][j].digit != third
                        && initialField[i][j].digit != fourth
                        && initialField[i][j].digit != fifth
                        && initialField[i][j].digit != sixth
                        && initialField[i][j].digit != seventh
                        && initialField[i][j].digit != eighth) {

                        currentField[i][j].digit = initialField[i][j].digit;
                        currentField[i][j].noEdit = true;

                        int box = 3 * (i / 3) + (j / 3);

                        areaRows[i].addDigit(currentField[i][j].digit, j, i);
                        areaCols[j].addDigit(currentField[i][j].digit, j, i);
                        areaBoxes[box].addDigit(currentField[i][j].digit, j, i);
                    }
                }
            }
            break;
        }
    }

}