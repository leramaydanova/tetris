#include "forms.h"

void createForm(form_t *form, int size, int posAmount, int type)
{
    form->form = calloc(4, sizeof(int **));
    for (int i = 0; i < posAmount; i++)
    {
        form->form[i] = calloc(size, sizeof(int *));
        for (int j = 0; j < size; j++) {
            form->form[i][j] = calloc(size, sizeof(int));
        }
    }
    form->posX = (10 - size) / 2;
    form->posY = 1;
    form->size = size;
    form->posAmount = posAmount;
    form->pos = 0;
    form->type = type;
}

// void formGeneration(form_t *form, Figures_t *figures)
// {
    
//     switch (formNum)
//     {
//     case 0: // переименовать по формам
//         {
//             int size = 3;
//             int amount = 4;
//             createForm(form, size, amount);
//             int formFill[4][3][3] = {
//                 {{0, 1, 0},
//                  {1, 1, 1},
//                  {0, 0, 0}},
//                 {{0, 1, 0},
//                  {0, 1, 1},
//                  {0, 1, 0}},
//                 {{0, 0, 0},
//                  {1, 1, 1},
//                  {0, 1, 0}},
//                 {{0, 1, 0},
//                  {1, 1, 0},
//                  {0, 1, 0}},
//             };
//             for (int n = 0; n < amount; n++) {
//                 for (int i = 0; i < size; i++) {
//                     for (int j = 0; j < size; j++) {
//                         form->form[n][i][j] = formFill[n][i][j];
//                     }
//                 } 
//             }
//             break;
//         }
//     case 1:
//         {
//             int size = 2;
//             int amount = 1;
//             createForm(form, size, amount);
//             for (int i = 0; i < size; i++) {
//                 form->form[0][0][i] = 1;
//                 form->form[0][1][i] = 1;
//             }
//             break;
//         }
//     case 2:
//         {
//             int size = 3;
//             int amount = 4;
//             createForm(form, size, amount);
//             int formFill[4][3][3] = {
//                 {{0, 1, 0},
//                  {0, 1, 0},
//                  {0, 1, 1}},
//                 {{0, 0, 0},
//                  {1, 1, 1},
//                  {1, 0, 0}},
//                 {{1, 1, 0},
//                  {0, 1, 0},
//                  {0, 1, 0}},
//                 {{0, 0, 1},
//                  {1, 1, 1},
//                  {0, 0, 0}},
//             };
//             for (int n = 0; n < amount; n++) {
//                 for (int i = 0; i < size; i++) {
//                     for (int j = 0; j < size; j++) {
//                         form->form[n][i][j] = formFill[n][i][j];
//                     }
//                 } 
//             }
//             break;
//         }
//     case 3: {
//         int size = 4;
//         int amount = 2;
//         createForm(form, size, amount);
//         int formFill[2][4][4] = {
//             {{0, 0, 1, 0},
//                 {0, 0, 1, 0},
//                 {0, 0, 1, 0},
//                 {0, 0, 1, 0}},
//                 {{0, 0, 0, 0},
//                 {0, 0, 0, 0},
//                 {1, 1, 1, 1},
//                 {0, 0, 0, 0}},
//         };
//         for (int n = 0; n < amount; n++)
//         {
//             for (int i = 0; i < size; i++)
//             {
//                 for (int j = 0; j < size; j++)
//                 {
//                     form->form[n][i][j] = formFill[n][i][j];
//                 }
//             }
//         }
//         break;
//     }
//     default:
//         break;
//     }
// }

void formGeneration(form_t *form, FigureType_t type)
{
    int formNum;
    if (type == -1) {
        srand(time(NULL));
        formNum = rand() % 4;
    } else {
        formNum = type;
    }

    switch (formNum)
    {
    case Tfigure: // переименовать по формам
    {
        int size = 3;
        int amount = 4;
        createForm(form, size, amount, formNum);
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
        createForm(form, size, amount, formNum);
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
        createForm(form, size, amount, formNum);
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
        createForm(form, size, amount, formNum);
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

void formUpdate(Figures_t *figures) { // test
    srand(time(NULL));
    int formNum = rand() % 4;
    figures->now = figures->next;
    figures->next = formNum;
}