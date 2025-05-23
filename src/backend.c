#include "backend.h"

void getUserInput(GameInfo_t *gi) {
    gi->action = -1;

    if (gi->state == START) timeout(-1);
    else timeout(10);

    int ch = getch();

    switch (ch) {
        case KEY_Q:
            gi->action = Start;
            break;
        case KEY_P:
            gi->action = Pause;
            break;
        case KEY_LEFT:
            gi->action = Left;
            break;
        case KEY_RIGHT:
            gi->action = Right;
            break;
        case KEY_UP:
            gi->action = Up;
            break;
        case KEY_R:
            gi->action = Action;
            break;
        case KEY_DOWN:
            gi->action = Down;
            break;
        case KEY_Z:
            gi->action = Terminate;
            break;
        default:
            break;
    }
}

void updateCurrentState(GameInfo_t *gi) {
    if (gi->state != START) {
        timer(gi);
    }
    
    if (gi->state == START && gi->action == Start) {
        gameInit(gi);
    }
    if (gi->state == SPAWN) {
        figuresGenerate(gi);
    }
    if (gi->state == MOVING) {
        moveFigure(gi);
    }
    if (gi->state == SHIFTING) {
        shift(gi);
    }
    if (gi->state == ATTACHING) {
        updateGameField(gi);
        deleteFullLines(gi);
    }
    if (gi->state == GAMEOVER || gi->action == Terminate) {
        finishGame(gi);
    }
}

void figuresGenerate(GameInfo_t *gi) {
    figuresUpdate(gi);
    int type1 = gi->figures.now;
    int type2 = gi->figures.next;
    if (gi->now.form) {
        freeForm(&(gi->now));
        freeForm(&(gi->next));
    }
    formGeneration(&(gi->now), type1);
    formGeneration(&(gi->next), type2);
    gi->state = MOVING;
}

void figuresUpdate(GameInfo_t *gi) {
    srand(time(NULL));

    if (!&(gi->now)) {
        gi->figures.now = rand() % 4;
    } else {
        gi->figures.now = gi->figures.next;
    }
    gi->figures.next = rand() % 4;
}

void moveFigure(GameInfo_t *gi) {
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

void moveLeft(GameInfo_t *gi) {
    int noMove = checkLeftBorder(gi->field, gi->now);
    if (!noMove)
        gi->now.x--;
}

void moveRight(GameInfo_t *gi) {
    int noMove = checkRightBorder(gi->field, gi->now);
    if (!noMove)
        gi->now.x++;
}

void moveDown(GameInfo_t *gi) {
    gi->speed = 100;
}

void rotate(GameInfo_t *gi) {
    int noMove = checkRotateByBorder(gi->field, gi->now);
    if (!noMove)
        gi->now.pos = (gi->now.pos + 1) % gi->now.posAmount;
}

void shift(GameInfo_t *gi) {
    gi->now.y++;
    gi->state = MOVING;
}

void finishGame(GameInfo_t *gi) {
    freeForm(&(gi->now));
    freeForm(&(gi->next));

    gi->state = GAMEOVER;
}

void gameInit(GameInfo_t *gi) {
    gi->time = getTime();
    gi->speed = SPEED;
    fillFieldInfo(gi);
    gi->now.form = NULL;
    gi->next.form = NULL;
    gi->state = SPAWN;
}

long long getTime() {
    struct timeval t;
    gettimeofday(&t, NULL);

    return t.tv_sec * 1000 + t.tv_usec / 1000;
}

void timer(GameInfo_t *gi) {
    long long t = getTime();

    if (t - gi->time >= gi->speed) {
        gi->state = SHIFTING;
        gi->time = t;
    }
}

void updateGameField(GameInfo_t *gi) {
    for (int i = 0; i < gi->now.size; i++) {
        for (int j = 0; j < gi->now.size; j++) {
            if (gi->now.form[gi->now.pos][i][j]) {
                gi->field[i + gi->now.y][j + gi->now.x] = BLOCKCOLOR;
            }
        }
    }
    gi->state = SPAWN;
}

void deleteFullLines(GameInfo_t *gi) {
    timeout(100);
    int stopF = 0;
    for (int i = 2; i < HEIGHT - 1; i++) {
        stopF = 0;
        for (int j = 1; j < WIDTH - 1 && !stopF; j++) {
            if (gi->field[i][j] != BLOCKCOLOR)
                stopF = 1;
        }
        if (!stopF) {
            for (int m = 1; m < WIDTH - 1; m++) {
                for (int n = i; n >= 2; n--) {
                    int temp = gi->field[n][m];
                    gi->field[n][m] = gi->field[n - 1][m];
                    gi->field[n - 1][m] = temp;
                }
            }
            for (int j = 1; j < WIDTH - 1; j++) {
                gi->field[1][j] = ' ';
            }
        }
    }
    if (checkTopBorder(gi)) gi->state = GAMEOVER;
}

bool checkTopBorder(GameInfo_t *gi) {
    bool res = FALSE;
    for (int j = 1; j < WIDTH - 1 && !res; j++) {
        if (gi->field[1][j] == BLOCKCOLOR)
            res = TRUE;
    }
    return res;
}

bool checkLeftBorder(int field[HEIGHT][WIDTH], form_t form) {
    bool res = 0;

    int x = form.x;
    int y = form.y;

    int stopF = 0;

    for (int i = 0; i < form.size && !res; i++) {
        stopF = 0;
        for (int j = 0; j < form.size && !res && !stopF; j++) {
            if (form.form[form.pos][i][j]) {
                if (field[i + y][j + x - 1] != ' ') {
                    res = TRUE;
                }
                stopF = 1;
            }
        }
    }
    return res;
}

bool checkRightBorder(int field[HEIGHT][WIDTH], form_t form) {
    bool res = FALSE;

    int x = form.x;
    int y = form.y;

    int stopF = 0;

    for (int i = 0; i < form.size && !res; i++) {
        stopF = 0;
        for (int j = form.size - 1; j >= 0 && !res && !stopF; j--) {
            if (form.form[form.pos][i][j]) {
                if (field[i + y][j + x + 1] != ' ') {
                    res = TRUE;
                }
                stopF = 1;
            }
        }
    }
    return res;
}

bool checkBottomBorder(int field[HEIGHT][WIDTH], form_t form) {
    bool res = 0;

    int x = form.x;
    int y = form.y;

    int stopF = 0;

    for (int j = 0; j < form.size && !res; j++) {
        stopF = 0;
        for (int i = form.size - 1; i >= 0 && !res && !stopF; i--) {
            if (form.form[form.pos][i][j]) {
                if (field[i + y + 1][j + x] != ' ') {
                    res = TRUE;
                }
                stopF = 1;
            }
        }
    }
    return res;
}

bool checkRotateByBorder(int field[HEIGHT][WIDTH], form_t form) {
    int currentPos = form.pos;
    int nextPos = (form.pos + 1) % form.posAmount;

    bool res = FALSE;

    int x = form.x;
    int y = form.y;

    for (int i = 0; i < form.size && !res; i++) {
        for (int j = 0; j < form.size && !res; j++) {
            if (form.form[nextPos][i][j] && !form.form[currentPos][i][j]) {
                if (i + y >= HEIGHT - 1 || i + y >= HEIGHT - 1 || j + x <= 0 || j + x >= WIDTH - 1 || field[i + y][j + x] != ' ') {
                    res = TRUE;
                }
            }
        }
    }
    return res;
}

void createForm(form_t *form, int size, int posAmount, int type) {
    form->form = calloc(4, sizeof(int **));
    for (int i = 0; i < posAmount; i++)
    {
        form->form[i] = calloc(size, sizeof(int *));
        for (int j = 0; j < size; j++) {
            form->form[i][j] = calloc(size, sizeof(int));
        }
    }
    form->x = (10 - size) / 2;
    form->y = 1;
    form->size = size;
    form->posAmount = posAmount;
    form->pos = 0;
    form->type = type;
}

void formGeneration(form_t *form, FigureType_t type) {
    switch (type) {
    case Tfigure: // добавить фцию поворота фигуры, изменить массив фигуры на **, добавить оставшиеся фигуры
    {
        int size = 3;
        int amount = 4;
        createForm(form, size, amount, type);
        int formFill[4][3][3] = {
            {{0, 1, 0},
             {1, 1, 1},
             {0, 0, 0}},
            {{0, 1, 0},
             {0, 1, 1},
             {0, 1, 0}},
            {{0, 0, 0},
             {1, 1, 1},
             {0, 1, 0}},
            {{0, 1, 0},
             {1, 1, 0},
             {0, 1, 0}},
        };
        for (int n = 0; n < amount; n++)
        {
            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    form->form[n][i][j] = formFill[n][i][j];
                }
            }
        }
        break;
    }
    case Ofigure:
    {
        int size = 2;
        int amount = 1;
        createForm(form, size, amount, type);
        for (int i = 0; i < size; i++)
        {
            form->form[0][0][i] = 1;
            form->form[0][1][i] = 1;
        }
        break;
    }
    case Lfigure:
    {
        int size = 3;
        int amount = 4;
        createForm(form, size, amount, type);
        int formFill[4][3][3] = {
            {{0, 1, 0},
             {0, 1, 0},
             {0, 1, 1}},
            {{0, 0, 0},
             {1, 1, 1},
             {1, 0, 0}},
            {{1, 1, 0},
             {0, 1, 0},
             {0, 1, 0}},
            {{0, 0, 1},
             {1, 1, 1},
             {0, 0, 0}},
        };
        for (int n = 0; n < amount; n++)
        {
            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    form->form[n][i][j] = formFill[n][i][j];
                }
            }
        }
        break;
    }
    case Ifigure:
    {
        int size = 4;
        int amount = 2;
        createForm(form, size, amount, type);
        int formFill[2][4][4] = {
            {{0, 0, 1, 0},
             {0, 0, 1, 0},
             {0, 0, 1, 0},
             {0, 0, 1, 0}},
            {{0, 0, 0, 0},
             {0, 0, 0, 0},
             {1, 1, 1, 1},
             {0, 0, 0, 0}},
        };
        for (int n = 0; n < amount; n++)
        {
            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    form->form[n][i][j] = formFill[n][i][j];
                }
            }
        }
        break;
    }
    default:
        break;
    }
}

void freeForm(form_t * form) {
    for (int n = 0; n < form->posAmount; n++) {
        for (int i = 0; i < form->size; i++) {
            free(form->form[n][i]);
        }
        free(form->form[n]);
    }
}

void fillFieldInfo(GameInfo_t *info) {
    for (int i = 1; i < 21; i++)
    {
        for (int j = 1; j < 11; j++)
        {
            info->field[i][j] = ' ';
        }
    }
    for (int j = 1; j < 11; j++)
    {
        info->field[0][j] = 0x2500; // дефами прописать коды
        info->field[21][j] = 0x2500;
    }
    for (int i = 1; i < 21; i++)
    {
        info->field[i][0] = 0x2502;
        info->field[i][11] = 0x2502;
    }

    info->field[0][0] = 0x250C;
    info->field[0][11] = 0x2510;
    info->field[21][0] = 0x2514;
    info->field[21][11] = 0x2518;
}