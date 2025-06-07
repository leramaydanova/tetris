#include "frontend.h"

void printStart();

void render(GameInfo_t *gi, GameState_t *gs, State_t state)
{
    clear();
    refresh();

    if (state == START) {
        printStart();
    }
        
    else if (state == GAMEOVER)
        showGameOver();

    else {
        printField(gi, gs);

        showNextFigure(gi);
    }
}

void printStart() {
    WINDOW *startMenu = newwin(20, 30, 0, 0);
    box(startMenu, 0, 0);

    int x, y;

    getmaxyx(startMenu, y, x);

    mvwprintw(startMenu, y / 2, (x - 17) / 2, "Welcome to Tetris");
    mvwprintw(startMenu, y / 2, (x - 18) / 2, "To start press 'q'");
    wrefresh(startMenu);
    delwin(startMenu);
}

// void showStart() {
//     char s[] = "GAME START Q";

//     int x, y;
//     getmaxyx(stdscr, y, x);
//     mvprintw(y / 2, (x - strlen(s)) / 2, "%s", s);
// }

void showGameOver() {
    WINDOW *gameOverWindow = newwin(20, 30, 0, 0);
    box(gameOverWindow, 0, 0);

    int x, y;

    getmaxyx(gameOverWindow, y, x);

    mvwprintw(gameOverWindow, y / 2 - 1, (x - 12) / 2, "GAME OVER :(");
    wrefresh(gameOverWindow);
    delwin(gameOverWindow);
    napms(2000);
}

void printField(GameInfo_t* gi, GameState_t* gs) {
    WINDOW *fieldWindow = newwin(HEIGHT + 2, WIDTH + 2, 0, 0);
    box(fieldWindow, 0, 0);

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (gi->field[i][j])
                mvwprintw(fieldWindow, i + 1, j + 1, "%lc", BLOCKCOLOR);
        }
    }

    for (int i = 0; i < gs->now.size; i++) {
        for (int j = 0; j < gs->now.size; j++) {
            if (gs->now.form[i][j])
                mvwprintw(fieldWindow, i + 1 + gs->now.y, j + 1 + gs->now.x, "%lc", BLOCKCOLOR);
        }
    }

    wrefresh(fieldWindow);
    delwin(fieldWindow);
}

void printInfo(GameInfo_t *gi) {
    
}

void fillField(GameInfo_t *gi, int dynamicField[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++){
            dynamicField[i][j] = gi->field[i][j];
        }
    }
}

void showNextFigure(GameInfo_t *gi) {
    WINDOW *nextFigureWindow = newwin(10, 15, 12, WIDTH + 5);
    box(nextFigureWindow, 0, 0);
    mvwprintw(nextFigureWindow, 1, 2, "Next figure:");

    int size = 0;
    while (gi->next[size++])
        ;

    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - 1; j++) {
            if (gi->next[i][j])
                mvwprintw(nextFigureWindow, 3 + i, 3 + j, "%lc", BLOCKCOLOR);
        }
    }

    wrefresh(nextFigureWindow);
    delwin(nextFigureWindow);
}