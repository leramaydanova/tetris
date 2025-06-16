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
#define FIGURE_SIZE 4

#include "../../tetris.h"

void createFigure(Figure_t *, int, int);
void turnForm(Figure_t *);
void spawnFigures(GameState_t *gi);
void figuresUpdate(GameState_t *gi);

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
void rotate90(Figure_t src, Figure_t *dest, int size);

void shift(GameState_t *gi);
void finishGame(GameState_t *gi);
void gameInit(GameState_t *gi);
long long getTime();
void timer(GameState_t *gi);
void updateGameField(GameState_t *gi);
void deleteFullLines(GameState_t *gi);

bool checkTopBorder(GameState_t *gi);
bool checkLeftBorder(int **field, Figure_t form); // переделать на передачу структуры игры
bool checkRightBorder(int **field, Figure_t form);
bool checkBottomBorder(int **field, Figure_t form);
bool checkRotateByBorder(int **field, Figure_t form);

void figureGenerate(Figure_t *form, FigureType_t type);
void freeFigure(Figure_t *form); 

void updateField(GameState_t *gs);

int scoring(int num);
void getHighScore(int num);
void updateHighScore(GameInfo_t *gi);

#endif