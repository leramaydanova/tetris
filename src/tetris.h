#ifndef TETRIS_H
#define TETRIS_H

#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


#define WIDTH 10
#define HEIGHT 20
#define BLOCKCOLOR 0x2588

#define WIN_INIT()         \
    setlocale(LC_ALL, ""); \
    initscr();             \
    nodelay(stdscr, true); \
    keypad(stdscr, true);  \
    noecho(); \
    curs_set(0)

typedef enum
{
    Tfigure,
    Ofigure,
    Lfigure,
    Ifigure,
    Sfigure,
    Zfigure,
    Jfigure,
} FigureType_t;

typedef struct
{
    FigureType_t now;
    FigureType_t next;
} Figures_t;

typedef struct
{
    int **form;
    int size;
    int x;
    int y;
    FigureType_t type;
} form_t;

typedef enum
{
    START,
    PAUSE,
    SPAWN,
    MOVING,
    SHIFTING,
    ATTACHING,
    GAMEOVER,
} State_t;

typedef enum
{
    Start,
    Pause,
    Terminate,
    Left,
    Right,
    Up,
    Down,
    Action,
} UserAction_t;

// typedef struct
// {
//     int field[HEIGHT][WIDTH]; // сменить на **
//     Figures_t figures;
//     form_t next;
//     double speed;
//     State_t state;
//     UserAction_t action;
//     long long time;
// } GameInfo_t;

typedef struct
{
    int **field; // сменить на **
    int **next;
    int score;
    int high_score;
    int level;
    int speed;
    int pause;
} GameInfo_t;

#include "backend.h"
#include "frontend.h"

void gameLoop();

#endif