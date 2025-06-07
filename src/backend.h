#ifndef BACKEND_H
#define BACKEND_H

#include <sys/time.h>
#include <time.h>

#define POS_X 5
#define POS_Y 1

#define KEY_R 114
#define KEY_P 112
#define KEY_Q 113
#define KEY_Z 122

#define NO_INPUT -1

#define SPEED 1000

#define POS_AMOUNT 7

#include "tetris.h"

void createFigure(form_t *, int);
void turnForm(form_t *);
void spawnFigures(GameState_t *gi);
void figuresUpdate(GameState_t *gi);

// void userInput(GameInfo_t *gi);
// void updateCurrentState(GameInfo_t *gi);
GameInfo_t updateCurrentState();
UserAction_t getAction();
GameState_t *getGame();
State_t* getState();

void userInput(UserAction_t action, bool hold);

void moveFigure(GameState_t *gi);
void moveLeft(GameState_t *gi);
void moveRight(GameState_t *gi);
void moveDown(GameState_t *gi);
void rotate(GameState_t *gi);
void rotate90(form_t src, form_t *dest, int size);

void shift(GameState_t *gi);
void finishGame(GameState_t *gi);
void gameInit(GameState_t *gi);
long long getTime();
void timer(GameState_t *gi);
void updateGameField(GameState_t *gi);
void deleteFullLines(GameState_t *gi);

bool checkTopBorder(GameState_t *gi);
bool checkLeftBorder(int **field, form_t form); // переделать на передачу структуры игры
bool checkRightBorder(int **field, form_t form);
bool checkBottomBorder(int **field, form_t form);
bool checkRotateByBorder(int **field, form_t form);

void figureGenerate(form_t *form, FigureType_t type);
void freeFigure(form_t *form); 

void fillFieldInfo(GameState_t *info);

#endif