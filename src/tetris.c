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

        userInput(getAction(), TRUE);

        GameInfo_t game = updateCurrentState();
        State_t *state = getState();

        render(&game, *state);

        if (*state == GAMEOVER)
            stopF = TRUE;


        // if (!stopF) {
            
        // }

    }
}