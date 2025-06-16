#ifndef FRONTEND_H
#define FRONTEND_H

#include "../../tetris.h"

void render(GameInfo_t* gi, State_t state);
void showGameOver(); 
void printField(GameInfo_t* gi);
void printInfo(GameInfo_t *gi);
void printStart();
void showNextFigure(GameInfo_t *gi);
void printPauseScreen();

#endif