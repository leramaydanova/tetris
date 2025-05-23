#include "tetris.h"

int main(void) {
    WIN_INIT();
    gameLoop();
    endwin();
    return 0;
}

void gameLoop() {
    GameInfo_t game;
    game.state = START;

    bool stopF = FALSE;

    while (!stopF) {

        if (game.state == GAMEOVER)
            stopF = TRUE;

        render(&game);

        if (!stopF) {
            getUserInput(&game);
        
            updateCurrentState(&game);
        }

    }
}