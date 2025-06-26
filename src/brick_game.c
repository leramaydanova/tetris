#include "brick_game.h"

int main(void) {
  WIN_INIT();
  gameLoop();
  endwin();
  return 0;
}

void gameLoop() {
  bool stopF = FALSE;

  while (!stopF) {
    userInput(getAction());

    GameInfo_t gi = updateCurrentState();
    State_t const *state = getState();

    render(&gi, *state);

    if (*state == GAMEOVER && gi.field == NULL) stopF = TRUE;
  }
}