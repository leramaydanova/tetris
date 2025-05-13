#include "forms.h"
#include "gameField.h"

#include <stdlib.h>
#include <math.h>

#define END_BTN 'q'

void printField(int field[HEIGHT][WIDTH]);
int checkLeftBorder(int field[HEIGHT][WIDTH], form_t form);
int checkRightBorder(int field[HEIGHT][WIDTH], form_t form);
int checkBottomBorder(int field[HEIGHT][WIDTH], form_t form);
int checkRotateByBorder(int field[HEIGHT][WIDTH], form_t form);
void deleteFillLines(FieldInfo_t *info);

int main(void)
{

    setlocale(LC_ALL, "");
    initscr();
    nodelay(stdscr, true);
    keypad(stdscr, true);
    curs_set(0);
    showMainMenu();
    timeout(-1);
    int q = getch();
    if (q == 'q') {
        clear();
    }
    form_t current;
    form_t next;

    formGeneration(&current, -1);
    formGeneration(&next, -1);
    Figures_t figures = {.now = current.type, .next = next.type};
    int field[HEIGHT][WIDTH] = {0};
    FieldInfo_t fieldInfo;
    fillFieldInfo(&fieldInfo);
    struct timespec start, now;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < next.size; i++)
    {
        for (int j = 0; j < next.size; j++)
        {
            mvprintw(5 + i, 20 + j, "%lc", next.form[0][i][j]);
        }
    }
    int stopF = 0;
    // создать отображение следующей формы
    while (!stopF)
    {
        double speed = 0.6; 
        updateField(fieldInfo, current, field);
        printField(field);
        if (checkBottomBorder(field, current)) {
            updateFieldInfo(&fieldInfo, current);
            freeForm(&current);
            freeForm(&next);
            formUpdate(&figures);
            formGeneration(&current, figures.now);
            formGeneration(&next, figures.next);
            deleteFillLines(&fieldInfo);
            
        }
        for (int i = 0; i < next.size; i++) {
            for (int j = 0; j < next.size; j++) {
                if (next.form[0][i][j]) mvprintw(5 + i, 20 + j, "%lc", BLOCKCOLOR);
            }
        }
        timeout(10);
        UserUnput_t action = getch();
        switch (action) {
            case STOP_BTN: {
                endwin();
                stopF = 1;
                break;
            }
            case LEFT_BTN: {
                if (!checkLeftBorder(field, current))
                    current.posX--;
                break;
            }
            case RIGTH_BTN: {
                if (!checkRightBorder(field, current))
                    current.posX++;
                break;
            }
            case ROTATE_BTN: {
                if (!checkRotateByBorder(field, current)) current.pos = (current.pos + 1) % current.posAmount;
                break;
            }
            case DOWN_BTN: {
                speed = 0.05;
                break;
            }
            default:
                break;
        }

        clock_gettime(CLOCK_MONOTONIC, &now);
        double elapsed = (now.tv_sec - start.tv_sec) + (now.tv_nsec - start.tv_nsec) / 1e9;
        
        if (elapsed >= speed)
        {
            current.posY++;
            start = now;
        }
        clear();
    }
    return 0;
}

void printField(int field[HEIGHT][WIDTH])
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            mvprintw(i, j, "%lc", field[i][j]);
        }
    }
}

// void fillField(int **field) {
//     for (int i = 0; i < 22; i++)
//     {
//         for (int j = 0; j < 12; j++)
//         {
//             field[i][j] = ' ';
//         }
//     }
//     for (int j = 1; j < 11; j++)
//     {
//         field[0][j] = 0x2500;
//         field[21][j] = 0x2500;
//     }
//     for (int i = 1; i < 21; i++)
//     {
//         field[i][0] = 0x2502;
//         field[i][11] = 0x2502;
//     }

//     field[0][0] = 0x250C;
//     field[0][11] = 0x2510;
//     field[21][0] = 0x2514;
//     field[21][11] = 0x2518;
// }

int checkLeftBorder(int field[HEIGHT][WIDTH], form_t form) // красво обьединить две функции ?
{
    int res = 0;

    int x = form.posX;
    int y = form.posY;

    int stopF = 0;

    for (int i = 0; i < form.size && !res; i++)
    {
        stopF = 0;
        for (int j = 0; j < form.size && !res && !stopF; j++)
        {
            if (form.form[form.pos][i][j]) {
                if (field[i + y][j + x - 1] != ' ') {
                    res = 1;
                }
                stopF = 1;
            }
        }
    }
    return res;
}

int checkRightBorder(int field[HEIGHT][WIDTH], form_t form)
{
    int res = 0;

    int x = form.posX;
    int y = form.posY;

    int stopF = 0;

    for (int i = 0; i < form.size && !res; i++)
    {
        stopF = 0;
        for (int j = form.size - 1; j >= 0 && !res && !stopF; j--)
        {
            if (form.form[form.pos][i][j])
            {
                if (field[i + y][j + x + 1] != ' ')
                {
                    res = 1;
                }
                stopF = 1;
            }
        }
    }
    return res;
}

int checkBottomBorder(int field[HEIGHT][WIDTH], form_t form) {
    int res = 0;

    int x = form.posX;
    int y = form.posY;

    int stopF = 0;

    for (int j = 0; j < form.size && !res; j++)
    {
        stopF = 0;
        for (int i = form.size - 1; i >= 0 && !res && !stopF; i--)
        {
            if (form.form[form.pos][i][j])
            {
                if (field[i + y + 1][j + x] != ' ')
                {
                    res = 1;
                }
                stopF = 1;
            }
        }
    }
    return res;
}

int checkRotateByBorder (int field[HEIGHT][WIDTH], form_t form) {
    int currentPos = form.pos;
    int nextPos = (form.pos + 1) % form.posAmount;

    int res = 0;

    int x = form.posX;
    int y = form.posY;

    for (int i = 0; i < form.size && !res; i++) {
        for (int j = 0; j < form.size && !res; j++)
        {
            if (form.form[nextPos][i][j] && !form.form[currentPos][i][j])
            {
                if (i + y >= HEIGHT - 1 || i + y >= HEIGHT - 1 || j + x <= 0 || j + x >= WIDTH - 1 || field[i + y][j + x] != ' ')
                {
                    res = 1;
                }
            }
        }
    }
    return res;
}

void deleteFillLines(FieldInfo_t *info)
{
    int stopF = 0;
    for (int i = 2; i < HEIGHT - 1; i++) {
        stopF = 0;
        for (int j = 1; j < WIDTH - 1 && !stopF; j++)
        {
            if (info->field[i][j] != BLOCKCOLOR)
                stopF = 1;
        }
        if (!stopF) {
            for (int m = 1; m < WIDTH - 1; m++) {
                for (int n = i; n >= 2; n--) {
                    int temp = info->field[n][m];
                    info->field[n][m] = info->field[n - 1][m];
                    info->field[n - 1][m] = temp;
                }
            }
            for (int j = 1; j < WIDTH - 1; j++) {
                info->field[1][j] = ' ';
            }
        }
    }
}

