#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include "forms.h"

#define WIDTH 12
#define HEIGHT 22
#define BLOCKCOLOR 0x25A0

// где-то надо добавить состояния игры

typedef struct {
    int field[HEIGHT][WIDTH];
} FieldInfo_t;

typedef struct {
    int score;
} GameInfo_t;

typedef enum {
    START,
    PAUSE,
    LEFT,
    RIGTH,
    DOWN,
    ROTATE,
    COLLIDE
} State_t;

typedef enum
{
    START_BTN = 'q',
    PAUSE_BTN = ' ',
    LEFT_BTN = KEY_LEFT,
    RIGTH_BTN = KEY_RIGHT,
    DOWN_BTN = KEY_DOWN,
    ROTATE_BTN = KEY_UP,
    STOP_BTN = 'x',
} UserUnput_t;

void fillField(FieldInfo_t info, int dynamicField[HEIGHT][WIDTH]);
void fillFieldInfo(FieldInfo_t *info);
void updateField(FieldInfo_t info, form_t form, int field[HEIGHT][WIDTH]);
void updateFieldInfo(FieldInfo_t *info, form_t form);
void showMainMenu();

#endif