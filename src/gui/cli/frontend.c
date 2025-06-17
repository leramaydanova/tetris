#include "frontend.h"

void render(GameInfo_t *gi, State_t state) {
  if (gi->pause)
    printPauseScreen();

  else {
    clear();
    refresh();

    if (state == START) {
      printStart();
    }

    else if (state == GAMEOVER)
      showGameOver();

    else {
      printField(gi);
      printInfo(gi);
      showNextFigure(gi);
      printControls();
    }
  }
}

void printPauseScreen() {
  WINDOW *pauseWindow = newwin(3, 10, 9, 1);
  box(pauseWindow, 0, 0);

  mvwprintw(pauseWindow, 1, 2, "PAUSE!");
  wrefresh(pauseWindow);
  delwin(pauseWindow);
}

void printStart() {
  int x = 30, y = 20;
  WINDOW *startMenu = newwin(y, x, 0, 0);
  box(startMenu, 0, 0);

  mvwprintw(startMenu, y / 2 - 1, (x - 17) / 2, "WELCOME TO TETRIS!");
  mvwprintw(startMenu, y / 2 + 1, (x - 18) / 2, "To start press 'q'");
  mvwprintw(startMenu, y - 2, (x - 17) / 2 + 1, "To exit press 'z'");
  wrefresh(startMenu);
  delwin(startMenu);
}

void showGameOver() {
  int x = 30, y = 20;

  WINDOW *gameOverWindow = newwin(y, x, 0, 0);
  box(gameOverWindow, 0, 0);
  mvwprintw(gameOverWindow, y / 2 - 1, (x - 12) / 2, "GAME OVER :(");
  wrefresh(gameOverWindow);
  delwin(gameOverWindow);
  napms(2000);
}

void printField(GameInfo_t *gi) {
  WINDOW *fieldWindow = newwin(HEIGHT + 2, WIDTH + 2, 0, 0);
  box(fieldWindow, 0, 0);

  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (gi->field[i][j])
        mvwprintw(fieldWindow, i + 1, j + 1, "%lc", BLOCKCOLOR);
    }
  }

  wrefresh(fieldWindow);
  delwin(fieldWindow);
}

void printInfo(GameInfo_t *gi) {
  WINDOW *infoWindow = newwin(5, 20, 0, WIDTH + 5);
  box(infoWindow, 0, 0);

  mvwprintw(infoWindow, 1, 1, "score: %d", gi->score);
  mvwprintw(infoWindow, 2, 1, "high score: %d", gi->high_score);
  mvwprintw(infoWindow, 3, 1, "level: %d", gi->level);
  wrefresh(infoWindow);
  delwin(infoWindow);
}

void showNextFigure(GameInfo_t *gi) {
  WINDOW *nextFigureWindow = newwin(8, 20, 5, WIDTH + 5);
  box(nextFigureWindow, 0, 0);
  mvwprintw(nextFigureWindow, 1, 2, "Next figure:");

  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      if (gi->next[i][j])
        mvwprintw(nextFigureWindow, 3 + i, 3 + j, "%lc", BLOCKCOLOR);
    }
  }

  wrefresh(nextFigureWindow);
  delwin(nextFigureWindow);
}

void printControls() {
  WINDOW *controlsWindow = newwin(9, 25, 13, WIDTH + 5);

  mvwprintw(controlsWindow, 1, 5, "CONTROLS:");
  mvwprintw(controlsWindow, 3, 0, "left: ←");
  mvwprintw(controlsWindow, 4, 0, "rigth: →");
  mvwprintw(controlsWindow, 5, 0, "down: ↓");
  mvwprintw(controlsWindow, 6, 0, "rotate: ↑(r)");
  mvwprintw(controlsWindow, 3, 11, "pause: p");
  mvwprintw(controlsWindow, 4, 11, "exit: z");
  wrefresh(controlsWindow);
  delwin(controlsWindow);
}