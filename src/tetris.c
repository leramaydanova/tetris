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

        GameState_t* gs = getGame();
        GameInfo_t gi = updateCurrentState();
        State_t *state = getState();

        render(&gi, gs, *state);


        if (*state == GAMEOVER)
            stopF = TRUE;


        // if (!stopF) {
            
        // }

    }
}