#ifndef FRONTEND_H
#define FRONTEND_H

#include "../../brick_game.h"

void render(GameInfo_t const *, State_t);
void showGameOver();
void printField(GameInfo_t const *);
void printInfo(GameInfo_t const *);
void printStart();
void showNextFigure(GameInfo_t const *);
void printPauseScreen();
void printControls();
void printWin(GameInfo_t const *);
void printGame(GameInfo_t const *);
void initColors();
void clearScreen();

#endif