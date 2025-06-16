#include "frontend.h"

void render(GameInfo_t *gi, State_t state)
{
    if (gi->pause && state != START && state != GAMEOVER)
        printPauseScreen();

    else {
        clear();
        refresh();    

        if (state == START) {
            printStart();
        }
            
        else if (state == GAMEOVER)
            showGameOver();

        else
        {
            printField(gi);
            printInfo(gi);
            showNextFigure(gi);
        }
    }

}

void printPauseScreen() {
    WINDOW *pauseWindow = newwin(4, WIDTH - 2, (HEIGHT - 10) / 2, 2);
    // HEIGHT + 2, WIDTH + 2,
    box(pauseWindow, 0, 0);

    mvwprintw(pauseWindow, 1, 2, "PAUSE");
    wrefresh(pauseWindow);
    delwin(pauseWindow);
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

void showGameOver() {
    int x = 30, y = 20;

    WINDOW *gameOverWindow = newwin(y, x, 0, 0);
    box(gameOverWindow, 0, 0);
    mvwprintw(gameOverWindow, y / 2 - 1, (x - 12) / 2, "GAME OVER :(");
    wrefresh(gameOverWindow);
    delwin(gameOverWindow);
    napms(2000);
}

void printField(GameInfo_t* gi) {
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
    WINDOW *infoWindow = newwin(7, 20, 0, WIDTH + 5);
    box(infoWindow, 0, 0);

    mvwprintw(infoWindow, 1, 1, "score: %d", gi->score);
    mvwprintw(infoWindow, 3, 1, "high score: %d", gi->high_score);
    mvwprintw(infoWindow, 5, 1, "level: %d", gi->level);
    wrefresh(infoWindow);
    delwin(infoWindow);
}

void showNextFigure(GameInfo_t *gi) {
    WINDOW *nextFigureWindow = newwin(10, 15, 12, WIDTH + 5);
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