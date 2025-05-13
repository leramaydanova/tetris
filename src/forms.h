#ifndef FORM_H
#define FORM_H

#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define POS_X 5
#define POS_Y 1

typedef enum {
    Tfigure,
    Ofigure,
    Lfigure,
    Ifigure,
} FigureType_t;

typedef struct
{
    FigureType_t now;
    FigureType_t next;
} Figures_t; // test

typedef struct {
    int ***form;
    int size; // сделать один size для всех, то есть убрать поле size
    int posX;
    int posY;
    int posAmount;
    int pos;
    FigureType_t type;
} form_t;

void createForm(form_t *, int, int, int);
void formGeneration(form_t *, FigureType_t); // сделать статическую память
void freeForm(form_t *);
void turnForm(form_t *);
void formUpdate(Figures_t *figures);
// void formGeneration(form_t *form, Figures_t *figures);

#endif