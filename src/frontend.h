#ifndef FRONTEND_H
#define FRONTEND_H

#include "tetris.h"

void render(GameInfo_t* gi, GameState_t *gs, State_t state);
void showGameOver();
void updateField(GameInfo_t *gi, int (*field)[WIDTH]);
void printField(GameInfo_t* gi, GameState_t* gs);
void fillField(GameInfo_t *gi, int dynamicField[HEIGHT][WIDTH]);
void showNextFigure(GameInfo_t *gi);

#endif