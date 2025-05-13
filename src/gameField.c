#include "gameField.h"
#include <string.h>

void fillField(FieldInfo_t info, int dynamicField[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            dynamicField[i][j] = info.field[i][j];
        }
    }
}

void fillFieldInfo(FieldInfo_t *info) {
    for (int i = 1; i < 21; i++)
    {
        for (int j = 1; j < 11; j++)
        {
            info->field[i][j] = ' ';
        }
    }
    for (int j = 1; j < 11; j++)
    {
        info->field[0][j] = 0x2500;
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

void updateField(FieldInfo_t info, form_t form, int field[HEIGHT][WIDTH])
{
    fillField(info, field);
    for (int i = 0; i < form.size; i++) {
        for (int j = 0; j < form.size; j++) {
            if (form.form[form.pos][i][j]) {
                field[i + form.posY][j + form.posX] = BLOCKCOLOR;
            }
        }
    }
}

void updateFieldInfo(FieldInfo_t *info, form_t form)
{
    for (int i = 0; i < form.size; i++)
    {
        for (int j = 0; j < form.size; j++)
        {
            if (form.form[form.pos][i][j])
            {
                info->field[i + form.posY][j + form.posX] = BLOCKCOLOR;
            }
        }
    }
    
}

void showMainMenu() {
    int x = 0, y = 0;
    char str[] = "GAME START 'Q'";
    getmaxyx(stdscr, y, x);
    mvprintw(y / 2, x / 2 - strlen(str), "%s", str);
}
