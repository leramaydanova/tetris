#include "frontend.h"

void printStart();

void render(GameInfo_t *gi)
{
    clear();
    refresh();

    if (gi->state == START) {
        printStart();
    }
        
    else if (gi->state == GAMEOVER)
        showGameOver();

    else {
        int field[HEIGHT][WIDTH];
        updateField(gi, field);

        printField(field);

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

void updateField(GameInfo_t *gi, int (*field)[WIDTH]) {
    fillField(gi, field); // можно один раз прописать в другом месте чтобы не перерисовывать 
    for (int i = 0; i < gi->now.size; i++) {
        for (int j = 0; j < gi->now.size; j++) {
            if (gi->now.form[i][j]) {
                field[i + gi->now.y][j + gi->now.x] = BLOCKCOLOR;
            }
        }
    }
}

void printField(int (*field)[WIDTH]) {
    WINDOW *fieldWindow = newwin(HEIGHT + 2, WIDTH + 2, 0, 0);
    box(fieldWindow, 0, 0);

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (field[i][j] == BLOCKCOLOR)
                mvwprintw(fieldWindow, i + 1, j + 1, "%lc", field[i][j]);
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
    WINDOW *nextFigureWindow = newwin(7, WIDTH, 12, WIDTH + 5);

    mvwprintw(nextFigureWindow, 0, 0, "Next figure:");

    for (int i = 0; i < gi->next.size; i++) {
        for (int j = 0; j < gi->next.size; j++) {
            if (gi->next.form[i][j])
                mvwprintw(nextFigureWindow, 3 + i, j, "%lc", BLOCKCOLOR);
        }
    }

    wrefresh(nextFigureWindow);
    delwin(nextFigureWindow);
}