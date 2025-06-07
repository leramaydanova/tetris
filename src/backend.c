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

State_t* getState() {
    static State_t state = START;
    return &state;
}

GameState_t* getGame() {
    static GameState_t game = {.state = START, .field = NULL};
    return &game;
}

void userInput(UserAction_t action, bool hold) {

    if (!hold) return; // ? why for

    GameState_t *game = getGame();
    game->action = action;

    if (game->state != START) {
        timer(game);
    }

    if (game->state == START && action == Start) {
        gameInit(game);
    }
    if (game->state == SPAWN) {
        spawnFigures(game);
    }
    if (game->state == MOVING) {
        cleanFigure(game);
        moveFigure(game);
    }
    if (game->state == SHIFTING) {
        cleanFigure(game);
        shift(game);
    }
    if (game->state == ATTACHING) {
        updateGameField(game);
        deleteFullLines(game);
    }
    if (game->state == GAMEOVER || action == Terminate) {
        finishGame(game);
    } 
}

void cleanFigure(GameState_t *gs) {
    for (int i = 0; i < gs->now.size; i++) {
        for (int j = 0; j < gs->now.size; j++) {
            if (gs->now.form[i][j])
                gs->field[i + gs->now.y][j + gs->now.x] = 0;
        }
    }
}

GameInfo_t updateCurrentState() {
    GameInfo_t gi;
    GameState_t *gs = getGame();
    State_t *state = getState();

    // потом убрать

    if (gs->field) {
        gi.field = gs->field;
        for (int i = 0; i < HEIGHT; i++) {
            gi.field[i] = gs->field[i];
        }
    }

    gi.next = gs->next.form;
    for (int i = 0; i < gs->next.size; i++){
        gi.next[i] = gs->next.form[i];
    }

    for (int i = 0; i < gs->now.size; i++) {
        for (int j = 0; j < gs->now.size; j++)
        {
            if (gs->now.form[i][j])
                gs->field[i + gs->now.y][j + gs->now.x] = 1;
        }
    }

    gi.speed = gs->speed;
    *state = gs->state;

    return gi;
}

    // void userInput(GameInfo_t *gi)
    // {
    //     gi->action = -1;

    //     // if (gi->state == START) timeout(-1);
    //     timeout(10);

    //     int ch = getch();

    //     switch (ch) {
    //         case KEY_Q:
    //             gi->action = Start;
    //             break;
    //         case KEY_P:
    //             gi->action = Pause;
    //             break;
    //         case KEY_LEFT:
    //             gi->action = Left;
    //             break;
    //         case KEY_RIGHT:
    //             gi->action = Right;
    //             break;
    //         case KEY_UP:
    //             gi->action = Up;
    //             break;
    //         case KEY_R:
    //             gi->action = Action;
    //             break;
    //         case KEY_DOWN:
    //             gi->action = Down;
    //             break;
    //         case KEY_Z:
    //             gi->action = Terminate;
    //             break;
    //         default:
    //             break;
    //     }
    // }

    // void updateCurrentState(GameInfo_t *gi) { // добавить появление верхней фигуры на экране
    //     if (gi->state != START) {
    //         timer(gi);
    //     }

    //     if (gi->state == START && gi->action == Start) {
    //         gameInit(gi);
    //     }
    //     if (gi->state == SPAWN) {
    //         spawnFigures(gi);
    //     }
    //     if (gi->state == MOVING) {
    //         moveFigure(gi);
    //     }
    //     if (gi->state == SHIFTING) {
    //         shift(gi);
    //     }
    //     if (gi->state == ATTACHING) {
    //         updateGameField(gi);
    //         deleteFullLines(gi);
    //     }
    //     if (gi->state == GAMEOVER || gi->action == Terminate) {
    //         finishGame(gi);
    //     }
    // }

void spawnFigures(GameState_t *gi)
{
    figuresUpdate(gi);
    FigureType_t type1 = gi->figures.now;
    FigureType_t type2 = gi->figures.next;
    if (gi->now.form) {
        freeFigure(&(gi->now));
        freeFigure(&(gi->next));
    }
    figureGenerate(&(gi->now), type1);
    figureGenerate(&(gi->next), type2);
    gi->state = MOVING;
}

void figuresUpdate(GameState_t *gi)
{
    if (!gi->now.form) {
        gi->figures.now = rand() % POS_AMOUNT;
    }
     else {
        gi->figures.now = gi->figures.next;
    }
    gi->figures.next = rand() % POS_AMOUNT;
}

void moveFigure(GameState_t *gi)
{
    switch(gi->action) {
        case Left:
            moveLeft(gi);
            break;
        case Right:
            moveRight(gi);
            break;
        case Down:
            moveDown(gi);
            break;
        case Up:
        case Action:
            rotate(gi);
            break;
        default:
            break;
    }

    if (gi->action != Down)
        gi->speed = SPEED;

    if (checkBottomBorder(gi->field, gi->now))
        gi->state = ATTACHING;
}

void moveLeft(GameState_t *gi)
{
    int noMove = checkLeftBorder(gi->field, gi->now);
    if (!noMove)
        gi->now.x--;
}

void moveRight(GameState_t *gi)
{
    int noMove = checkRightBorder(gi->field, gi->now);
    if (!noMove)
        gi->now.x++;
}

void moveDown(GameState_t *gi)
{
    gi->speed = 50;
}

void rotate(GameState_t *gi)
{
    int noMove = checkRotateByBorder(gi->field, gi->now);
    if (!noMove) {
        form_t temp;
        int size = gi->now.size;
        createFigure(&temp, size);
        rotate90(gi->now, &temp, size);
        temp.x = gi->now.x;
        temp.y = gi->now.y;

        freeFigure(&(gi->now));
        createFigure(&(gi->now), size);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                gi->now.form[i][j] = temp.form[i][j];
            }
        }
        gi->now.x = temp.x;
        gi->now.y = temp.y;
        freeFigure(&temp);
    }
}

void shift(GameState_t *gi)
{
    gi->now.y++;
    gi->state = MOVING;
}

void finishGame(GameState_t *gi)
{
    freeFigure(&(gi->now));
    freeFigure(&(gi->next));

    for (int i = 0; i < HEIGHT; i++)
        free(gi->field[i]);
    free(gi->field);

    gi->state = GAMEOVER;
}

void gameInit(GameState_t *gi)
{
    gi->time = getTime();
    gi->speed = SPEED;
    gi->now.form = NULL;
    gi->now.size = 0;
    gi->next.size = 0;
    gi->next.form = NULL;
    gi->state = SPAWN;

    gi->field = calloc(HEIGHT, sizeof(int *));
    for (int i = 0; i < HEIGHT; i++)
        gi->field[i] = calloc(WIDTH, sizeof(int));
}

long long getTime() {
    struct timeval t;
    gettimeofday(&t, NULL);

    return t.tv_sec * 1000 + t.tv_usec / 1000;
}

void timer(GameState_t *gi)
{
    long long t = getTime();

    if (t - gi->time >= gi->speed) {
        gi->state = SHIFTING;
        gi->time = t;
    }
}

void updateGameField(GameState_t *gi)
{
    for (int i = 0; i < gi->now.size; i++) {
        for (int j = 0; j < gi->now.size; j++) {
            if (gi->now.form[i][j]) {
                gi->field[i + gi->now.y][j + gi->now.x] = 1;
            }
        }
    }
    gi->state = SPAWN;
}

void deleteFullLines(GameState_t *gi)
{
    int stopF = 0;
    for (int i = 1; i < HEIGHT; i++) {
        stopF = 0;
        for (int j = 0; j < WIDTH && !stopF; j++) {
            if (gi->field[i][j] != 1)
                stopF = 1;
        }
        if (!stopF) {
            for (int m = 0; m < WIDTH; m++)
            {
                for (int n = i; n >= 1; n--) {
                    int temp = gi->field[n][m];
                    gi->field[n][m] = gi->field[n - 1][m];
                    gi->field[n - 1][m] = temp;
                }
            }
            for (int j = 0; j < WIDTH; j++) {
                gi->field[0][j] = 0;
            }
        }
    }
    if (checkTopBorder(gi)) gi->state = GAMEOVER;
}

bool checkTopBorder(GameState_t *gi)
{
    bool res = FALSE;
    for (int j = 0; j < WIDTH && !res; j++) {
        if (gi->field[0][j] == 1)
            res = TRUE;
    }
    return res;
}

bool checkLeftBorder(int **field, form_t form) {
    bool res = 0;

    int x = form.x;
    int y = form.y;

    int stopF = 0;

    for (int i = 0; i < form.size && !res; i++) {
        stopF = 0;
        for (int j = 0; j < form.size && !res && !stopF; j++) {
            if (form.form[i][j]) {
                if (j + x - 1 < 0 || field[i + y][j + x - 1] != 0) {
                    res = TRUE;
                }
                stopF = 1;
            }
        }
    }
    return res;
}

bool checkRightBorder(int **field, form_t form) {
    bool res = FALSE;

    int x = form.x;
    int y = form.y;

    int stopF = 0;

    for (int i = 0; i < form.size && !res; i++) {
        stopF = 0;
        for (int j = form.size - 1; j >= 0 && !res && !stopF; j--) {
            if (form.form[i][j]) {
                if (j + x + 1 >= WIDTH || field[i + y][j + x + 1] != 0)
                {
                    res = TRUE;
                }
                stopF = 1;
            }
        }
    }
    return res;
}

bool checkBottomBorder(int **field, form_t form) {
    bool res = FALSE;

    int x = form.x;
    int y = form.y;

    int stopF = 0;

    for (int j = 0; j < form.size && !res; j++) {
        stopF = 0;
        for (int i = form.size - 1; i >= 0 && !res && !stopF; i--) {
            if (form.form[i][j]) {
                if (i + y + 1 >= HEIGHT || field[i + y + 1][j + x] != 0)
                {
                    res = TRUE;
                }
                stopF = 1;
            }
        }
    }
    return res;
}

bool checkRotateByBorder(int **field, form_t form) {
    bool res = FALSE;

    int x = form.x;
    int y = form.y;

    form_t temp;
    int size = form.size;
    createFigure(&temp, size);
    rotate90(form, &temp, size);

    for (int i = 0; i < form.size && !res; i++) {
        for (int j = 0; j < form.size && !res; j++) {
            if (temp.form[i][j] && !form.form[i][j]) {
                if (i + y >= HEIGHT || j + x < 0 || j + x >= WIDTH || field[i + y][j + x] != 0) {
                    res = TRUE;
                }
            }
        }
    }
    return res;
}

void createFigure(form_t *form, int size) {
    form->form = calloc(size, sizeof(int *));
    for (int i = 0; i < size; i++) {
        form->form[i] = calloc(size, sizeof(int));
    }
    form->x = (WIDTH - size) / 2;
    form->y = 1;
    form->size = size;
}

void figureGenerate(form_t *form, FigureType_t type) {
    switch (type) {
    case Tfigure: // добавить оставшиеся фигуры, разбить по функциям создание фигур
    {
        int size = 3;
        createFigure(form, size);
        int formFill[3][3] = 
            {{0, 1, 0},
             {1, 1, 1},
             {0, 0, 0}};
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                form->form[i][j] = formFill[i][j];
            }
        }
        break;
    }
    case Ofigure:
    {
        int size = 2;
        createFigure(form, size);
        for (int i = 0; i < size; i++) {
            form->form[0][i] = 1;
            form->form[1][i] = 1;
        }
        break;
    }
    case Lfigure:
    {
        int size = 3;
        createFigure(form, size);
        int formFill[3][3] = 
            {{0, 1, 0},
             {0, 1, 0},
             {0, 1, 1}};
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                form->form[i][j] = formFill[i][j];
            }
        }
        break;
    }
    case Ifigure:
    {
        int size = 4;
        createFigure(form, size);
        int formFill[4][4] = 
            {{0, 0, 1, 0},
             {0, 0, 1, 0},
             {0, 0, 1, 0},
             {0, 0, 1, 0}};
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                form->form[i][j] = formFill[i][j];
            }
        }
        break;
    }
    case Sfigure: // добавить оставшиеся фигуры, разбить по функциям создание фигур
    {
        int size = 3;
        createFigure(form, size);
        int formFill[3][3] =
            {{0, 0, 0},
             {1, 1, 0},
             {0, 1, 1}};
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                form->form[i][j] = formFill[i][j];
            }
        }
        break;
    }
    case Zfigure: // добавить оставшиеся фигуры, разбить по функциям создание фигур
    {
        int size = 3;
        createFigure(form, size);
        int formFill[3][3] =
            {{0, 0, 0},
             {0, 1, 1},
             {1, 1, 0}};
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                form->form[i][j] = formFill[i][j];
            }
        }
        break;
    }
    case Jfigure: // добавить оставшиеся фигуры, разбить по функциям создание фигур
    {
        int size = 3;
        createFigure(form, size);
        int formFill[3][3] =
            {{0, 1, 0},
             {0, 1, 0},
             {1, 1, 0}};
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                form->form[i][j] = formFill[i][j];
            }
        }
        break;
    }
    default:
        break;
    }
}

void freeFigure(form_t * form) {
    for (int i = 0; i < form->size; i++) {
        free(form->form[i]);
    }
    free(form->form);

    form->form = NULL;
    form->size = 0;
}

void fillFieldInfo(GameState_t *info)
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            info->field[i][j] = 0;
        }
    }
}

void rotate90(form_t src, form_t *dest, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++)
        {
            dest->form[j][size - 1 - i] = src.form[i][j];
        }
    }
}