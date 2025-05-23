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

#define SPEED 1000

#define POS_AMOUNT 4

#include "tetris.h"


void createForm(form_t *, int, int, int);
void formGeneration(form_t *, FigureType_t);
void freeForm(form_t *);
void turnForm(form_t *);
void figuresGenerate(GameInfo_t *gi);
void figuresUpdate(GameInfo_t *gi);

void getUserInput(GameInfo_t *gi);
void updateCurrentState(GameInfo_t *gi);

void moveFigure(GameInfo_t *gi);
void moveLeft(GameInfo_t *gi);
void moveRight(GameInfo_t *gi);
void moveDown(GameInfo_t *gi);
void rotate(GameInfo_t *gi);

void shift(GameInfo_t *gi);
void finishGame(GameInfo_t *gi);
void gameInit(GameInfo_t *gi);
long long getTime();
void timer(GameInfo_t *gi);
void updateGameField(GameInfo_t *gi);
void deleteFullLines(GameInfo_t *gi);

bool checkTopBorder(GameInfo_t *gi);
bool checkLeftBorder(int field[HEIGHT][WIDTH], form_t form); // переделать на передачу структуры игры
bool checkRightBorder(int field[HEIGHT][WIDTH], form_t form);
bool checkBottomBorder(int field[HEIGHT][WIDTH], form_t form);
bool checkRotateByBorder(int field[HEIGHT][WIDTH], form_t form);

void createForm(form_t *form, int size, int posAmount, int type);
void formGeneration(form_t *form, FigureType_t type);
void freeForm(form_t *form);

void fillFieldInfo(GameInfo_t *info);

#endif