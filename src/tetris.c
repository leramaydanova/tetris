#include "tetris.h"

int main(void) {
    WIN_INIT();
    gameLoop();
    endwin();
    return 0;
}

void gameLoop() {

    bool stopF = FALSE;
    srand(time(NULL));

    while (!stopF) {

        GameInfo_t gi = updateCurrentState();
        State_t *state = getState();

        render(&gi, *state);

        userInput(getAction(), TRUE);


        if (*state == GAMEOVER)
            stopF = TRUE;

    }
}