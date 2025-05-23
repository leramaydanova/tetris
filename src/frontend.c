#include "frontend.h"

void render(GameInfo_t *gi) {
    clear();
    if (gi->state == START)
        showStart();
    else if (gi->state == GAMEOVER) showGameOver();

    else {
        int field[HEIGHT][WIDTH];
        updateField(gi, field);

        printField(field);

        showNextFigure(gi);
    }

    refresh();
}

void showStart() {
    char s[] = "GAME START Q";

    int x, y;
    getmaxyx(stdscr, y, x);
    mvprintw(y / 2, (x - strlen(s)) / 2, "%s", s);
}

void showGameOver() {
    napms(1000);
    char s[] = "GAME OVER";

    int x, y;
    getmaxyx(stdscr, y, x);
    mvprintw(y / 2, (x - strlen(s)) / 2, "%s", s);
    refresh();
    napms(2000);
}

void updateField(GameInfo_t *gi, int (*field)[WIDTH]) {
    fillField(gi, field); // можно один раз прописать в другом месте чтобы не перерисовывать 
    for (int i = 0; i < gi->now.size; i++) {
        for (int j = 0; j < gi->now.size; j++) {
            if (gi->now.form[gi->now.pos][i][j]) {
                field[i + gi->now.y][j + gi->now.x] = BLOCKCOLOR;
            }
        }
    }
}

void printField(int (*field)[WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            mvprintw(i, j, "%lc", field[i][j]);
        }
    }
}

void fillField(GameInfo_t *gi, int dynamicField[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++){
            dynamicField[i][j] = gi->field[i][j];
        }
    }
}

void showNextFigure(GameInfo_t *gi) {
    for (int i = 0; i < gi->next.size; i++) {
        for (int j = 0; j < gi->next.size; j++) {
            if (gi->next.form[0][i][j]) mvprintw(5 + i, 20 + j, "%lc", BLOCKCOLOR);
        }
    }
}