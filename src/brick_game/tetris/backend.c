#include "backend.h"

UserAction_t getAction() {
  UserAction_t action = NO_INPUT;

  timeout(10);

  int ch = getch();

  switch (ch) {
    case KEY_Q:
      action = Start;
      break;
    case KEY_P:
      action = Pause;
      break;
    case KEY_LEFT:
      action = Left;
      break;
    case KEY_RIGHT:
      action = Right;
      break;
    case KEY_UP:
      action = Up;
      break;
    case KEY_R:
      action = Action;
      break;
    case KEY_DOWN:
      action = Down;
      break;
    case KEY_Z:
      action = Terminate;
      break;
    default:
      break;
  }

  return action;
}

State_t *getState() {
  static State_t state = START;
  return &state;
}

GameState_t *getGame() {
  static GameState_t game = {.state = START, .field = NULL};
  return &game;
}

void cleanFigure(GameState_t *gs) {
  for (int i = 0; i < gs->now.size; i++) {
    for (int j = 0; j < gs->now.size; j++) {
      if (gs->now.form[i][j]) gs->field[i + gs->now.y][j + gs->now.x] = 0;
    }
  }
}

void userInput(UserAction_t action) {
  GameState_t *game = getGame();

  if (game->state != START && game->state != GAMEOVER && action == Pause) {
    if (game->pause)
      game->pause = 0;
    else {
      game->pause = 1;
    }
  }

  if (game->pause && action != Terminate) return;

  if (action == Terminate && game->state != START && game->state != GAMEOVER) {
    game->pause = 0;
    game->state = GAMEOVER;
  }

  game->action = action;

  if (game->state == START && action == Start) {
    gameInit(game);
  }
  if (game->state == SPAWN) {
    spawnFigures(game);
  }

  if (game->state != START && game->state != GAMEOVER) {
    cleanFigure(game);
    timer(game);
  }

  if (game->state == MOVING) {
    moveFigure(game);
  }

  if (game->state == SHIFTING) {
    shift(game);
  }

  if (game->state == ATTACHING) {
    updateGameField(game);
    deleteFullLines(game);
  } else if (game->state == GAMEOVER) {
    finishGame(game);
  }

  if (action == Terminate && game->state == START) game->state = GAMEOVER;
}

GameInfo_t updateCurrentState() {
  GameInfo_t gi;
  GameState_t *gs = getGame();
  State_t *state = getState();

  if (gs->state != SPAWN) {
    updateField(gs);
  }

  gi.field = gs->field;

  gi.next = gs->next.form;

  gi.speed = gs->speed;
  gi.score = gs->score;
  gi.high_score = gs->high_score;
  gi.level = gs->level;
  gi.pause = gs->pause;
  *state = gs->state;

  return gi;
}

void updateField(GameState_t *gs) {
  for (int i = 0; i < gs->now.size; i++) {
    for (int j = 0; j < gs->now.size; j++) {
      if (gs->now.form[i][j])
        gs->field[i + gs->now.y][j + gs->now.x] = gs->now.form[i][j];
    }
  }
}

void updateHighScore(GameState_t *gs) {
  FILE *file = fopen(PATH_SCORE, "rt");

  if (file) {
    fscanf(file, "%d", &gs->high_score);
    fclose(file);
  }
}

void spawnFigures(GameState_t *gs) {
  figuresUpdate(gs);
  FigureType_t type1 = gs->figures.now;
  FigureType_t type2 = gs->figures.next;
  if (gs->now.form) {
    freeFigure(&(gs->now));
    freeFigure(&(gs->next));
  }
  figureGenerate(&(gs->now), type1);
  figureGenerate(&(gs->next), type2);
  gs->state = MOVING;
}

void figuresUpdate(GameState_t *gs) {
  if (!gs->now.form) {
    gs->figures.now = clock() % POS_AMOUNT;
  } else {
    gs->figures.now = gs->figures.next;
  }
  gs->figures.next = clock() % POS_AMOUNT;
}

void moveFigure(GameState_t *gs) {
  switch (gs->action) {
    case Left:
      moveLeft(gs);
      break;
    case Right:
      moveRight(gs);
      break;
    case Down:
      moveDown(gs);
      break;
    case Up:
    case Action:
      rotate(gs);
      break;
    default:
      break;
  }

  if (gs->action != Down) gs->speed = SPEED + gs->addSpeed;

  if (checkBottomBorder(gs)) gs->state = ATTACHING;
}

void moveLeft(GameState_t *gs) {
  int noMove = checkLeftBorder(gs);
  if (!noMove) gs->now.x--;
}

void moveRight(GameState_t *gs) {
  int noMove = checkRightBorder(gs);
  if (!noMove) gs->now.x++;
}

void moveDown(GameState_t *gs) { gs->speed = 50; }

void rotate(GameState_t *gs) {
  int noMove = checkRotateByBorder(gs);
  if (!noMove) {
    Figure_t temp;
    int size = gs->now.size;
    createFigure(&temp, size, 0);
    rotate90(gs->now, &temp, size);
    temp.x = gs->now.x;
    temp.y = gs->now.y;

    freeFigure(&(gs->now));
    createFigure(&(gs->now), size, 0);
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        gs->now.form[i][j] = temp.form[i][j];
      }
    }
    gs->now.x = temp.x;
    gs->now.y = temp.y;
    freeFigure(&temp);
  }
}

void shift(GameState_t *gs) {
  gs->now.y++;
  gs->state = MOVING;
}

void finishGame(GameState_t *gs) {
  gs->state = GAMEOVER;

  freeFigure(&(gs->now));
  freeFigure(&(gs->next));

  for (int i = 0; i < HEIGHT; i++) free(gs->field[i]);
  free(gs->field);
  gs->field = NULL;
}

void gameInit(GameState_t *gs) {
  gs->time = getTime();
  gs->speed = SPEED;
  gs->addSpeed = 0;
  gs->now.form = NULL;
  gs->now.size = 0;
  gs->next.size = 0;
  gs->next.form = NULL;
  gs->state = SPAWN;
  gs->score = 0;
  gs->level = 0;
  gs->pause = 0;

  updateHighScore(gs);

  gs->field = calloc(HEIGHT, sizeof(int *));
  for (int i = 0; i < HEIGHT; i++) gs->field[i] = calloc(WIDTH, sizeof(int));
}

long long getTime() {
  struct timeval t;
  gettimeofday(&t, NULL);

  return t.tv_sec * 1000 + t.tv_usec / 1000;
}

void timer(GameState_t *gs) {
  long long t = getTime();

  if (t - gs->time >= gs->speed) {
    gs->state = SHIFTING;
    gs->time = t;
  }
}

void updateGameField(GameState_t *gs) {
  for (int i = 0; i < gs->now.size; i++) {
    for (int j = 0; j < gs->now.size; j++) {
      if (gs->now.form[i][j]) {
        gs->field[i + gs->now.y][j + gs->now.x] = 7;
      }
    }
  }
  gs->state = SPAWN;
}

void deleteFullLines(GameState_t *gs) {
  int lines = 0;
  for (int i = 0; i < HEIGHT; i++) {
    int stopF = 0;
    for (int j = 0; j < WIDTH && !stopF; j++) {
      if (gs->field[i][j] == 0) stopF = 1;
    }
    if (!stopF) {
      lines++;
      for (int n = i; n > 1; n--) {
        int *temp = gs->field[n];
        gs->field[n] = gs->field[n - 1];
        gs->field[n - 1] = temp;
      }
      for (int j = 0; j < WIDTH; j++) {
        gs->field[1][j] = 0;
      }
    }
  }

  scoring(gs, lines);

  if (checkTopBorder(gs) || gs->level == 10)
    gs->state = GAMEOVER;
  else
    gs->state = SPAWN;
}

void scoring(GameState_t *gs, int n) {
  gs->score += getScore(n);

  if (gs->score / 600 - gs->level) {
    gs->addSpeed -= 100;
    gs->level++;
  }

  getHighScore(gs);
}

int getScore(int num) {
  int res = 0;
  switch (num) {
    case 1:
      res += 100;
      break;
    case 2:
      res += 300;
      break;
    case 3:
      res += 700;
      break;
    case 4:
      res += 1500;
      break;
  }

  return res;
}

void getHighScore(GameState_t *gs) {
  if (gs->score > gs->high_score) {
    gs->high_score = gs->score;
    FILE *high = fopen(PATH_SCORE, "wt");

    if (high) {
      fprintf(high, "%d", gs->high_score);
      fclose(high);
    }
  }
}

bool checkTopBorder(GameState_t const *gs) {
  bool res = FALSE;
  for (int j = 0; j < WIDTH && !res; j++) {
    if (gs->field[0][j]) res = TRUE;
  }
  return res;
}

bool checkLeftBorder(GameState_t const *gs) {
  bool res = 0;
  int x = gs->now.x;
  int y = gs->now.y;

  for (int i = 0; i < gs->now.size && !res; i++) {
    int stopF = 0;
    for (int j = 0; j < gs->now.size && !res && !stopF; j++) {
      if (gs->now.form[i][j]) {
        if (j + x - 1 < 0 || gs->field[i + y][j + x - 1] != 0) {
          res = TRUE;
        }
        stopF = 1;
      }
    }
  }
  return res;
}

bool checkRightBorder(GameState_t const *gs) {
  bool res = FALSE;

  int x = gs->now.x;
  int y = gs->now.y;

  for (int i = 0; i < gs->now.size && !res; i++) {
    int stopF = 0;
    for (int j = gs->now.size - 1; j >= 0 && !res && !stopF; j--) {
      if (gs->now.form[i][j]) {
        if (j + x + 1 >= WIDTH || gs->field[i + y][j + x + 1] != 0) {
          res = TRUE;
        }
        stopF = 1;
      }
    }
  }
  return res;
}

bool checkBottomBorder(GameState_t const *gs) {
  bool res = FALSE;

  int x = gs->now.x;
  int y = gs->now.y;

  for (int j = 0; j < gs->now.size && !res; j++) {
    int stopF = 0;
    for (int i = gs->now.size - 1; i >= 0 && !res && !stopF; i--) {
      if (gs->now.form[i][j]) {
        if (i + y + 1 >= HEIGHT || gs->field[i + y + 1][j + x] != 0) {
          res = TRUE;
        }
        stopF = 1;
      }
    }
  }
  return res;
}

bool checkRotateByBorder(GameState_t const *gs) {
  bool res = FALSE;

  int x = gs->now.x;
  int y = gs->now.y;

  Figure_t temp;
  int size = gs->now.size;
  createFigure(&temp, size, 0);
  rotate90(gs->now, &temp, size);

  for (int i = 0; i < gs->now.size && !res; i++) {
    for (int j = 0; j < gs->now.size && !res; j++) {
      if (temp.form[i][j] && !gs->now.form[i][j]) {
        if (i + y < 0 || i + y >= HEIGHT || j + x < 0 || j + x >= WIDTH ||
            gs->field[i + y][j + x] != 0) {
          res = TRUE;
        }
      }
    }
  }

  freeFigure(&temp);

  return res;
}

void createFigure(Figure_t *form, int size, int add) {
  form->form = calloc(FIGURE_SIZE, sizeof(int *));
  for (int i = 0; i < FIGURE_SIZE; i++) {
    form->form[i] = calloc(FIGURE_SIZE, sizeof(int));
  }
  form->x = (WIDTH - size) / 2;
  form->y = 0 + add;
  form->size = size;
}

void figureGenerate(Figure_t *form, FigureType_t type) {
  switch (type) {
    case Tfigure: {
      int size = 3;
      createFigure(form, size, 0);
      form->form[0][1] = 1;
      form->form[1][0] = 1;
      form->form[1][1] = 1;
      form->form[1][2] = 1;
      break;
    }
    case Ofigure: {
      int size = 2;
      createFigure(form, size, 0);
      form->form[0][0] = 2;
      form->form[0][1] = 2;
      form->form[1][0] = 2;
      form->form[1][1] = 2;
      break;
    }
    case Lfigure: {
      int size = 3;
      createFigure(form, size, 0);
      form->form[0][1] = 3;
      form->form[1][1] = 3;
      form->form[2][1] = 3;
      form->form[2][2] = 3;
      break;
    }
    case Ifigure: {
      int size = 4;
      createFigure(form, size, 0);
      form->form[0][2] = 4;
      form->form[1][2] = 4;
      form->form[2][2] = 4;
      form->form[3][2] = 4;
      break;
    }
    case Sfigure: {
      int size = 3;
      createFigure(form, size, -1);
      form->form[1][0] = 5;
      form->form[1][1] = 5;
      form->form[2][1] = 5;
      form->form[2][2] = 5;
      break;
    }
    case Zfigure: {
      int size = 3;
      createFigure(form, size, -1);
      form->form[1][2] = 5;
      form->form[1][1] = 5;
      form->form[2][1] = 5;
      form->form[2][0] = 5;
      break;
    }
    case Jfigure: {
      int size = 3;
      createFigure(form, size, 0);
      form->form[0][1] = 6;
      form->form[1][1] = 6;
      form->form[2][1] = 6;
      form->form[2][0] = 6;
      break;
    }
    default:
      break;
  }
}

void freeFigure(Figure_t *form) {
  for (int i = 0; i < form->size; i++) {
    free(form->form[i]);
  }
  free(form->form);

  form->form = NULL;
  form->size = 0;
}

void rotate90(Figure_t src, Figure_t *dest, int size) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      dest->form[j][size - 1 - i] = src.form[i][j];
    }
  }
}