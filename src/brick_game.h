#ifndef BRICK_GAME_H
#define BRICK_GAME_H

#include <locale.h>
#include <math.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 10
#define HEIGHT 20

#define WIN_INIT()       \
  setlocale(LC_ALL, ""); \
  initscr();             \
  nodelay(stdscr, true); \
  keypad(stdscr, true);  \
  noecho();              \
  curs_set(0);           \
  start_color();         \
  initColors();

typedef enum {
  START,
  SPAWN,
  MOVING,
  SHIFTING,
  ATTACHING,
  GAMEOVER,
} State_t;

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action,
} UserAction_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

#include "brick_game/tetris/backend.h"
#include "gui/cli/frontend.h"

void gameLoop();

#endif