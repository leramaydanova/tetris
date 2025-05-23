#ifndef TETRIS_H
#define TETRIS_H

#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


#define WIDTH 12
#define HEIGHT 22
#define BLOCKCOLOR 0x2588

#define WIN_INIT()         \
    setlocale(LC_ALL, ""); \
    initscr();             \
    nodelay(stdscr, true); \
    keypad(stdscr, true);  \
    noecho(); \
    curs_set(0)

    typedef enum {
        Tfigure,
        Ofigure,
        Lfigure,
        Ifigure,
    } FigureType_t;

typedef struct
{
    FigureType_t now;
    FigureType_t next;
} Figures_t;

typedef struct
{
    int ***form;
    int size;
    int x;
    int y;
    int posAmount;
    int pos;
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
    Left,
    Right,
    Down,
    Rotate,
    Up,
    Terminate,
    Action,
} UserInput_t;

typedef struct
{
    int field[HEIGHT][WIDTH];
    Figures_t figures;
    form_t now;
    form_t next;
    double speed;
    State_t state;
    UserInput_t action;
    long long time;
} GameInfo_t;

#include "backend.h"
#include "frontend.h"

void gameLoop();

#endif