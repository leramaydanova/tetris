#ifndef BACKEND_H
#define BACKEND_H

#include <sys/time.h>
#include <time.h>

#define POS_X 5
#define POS_Y 1

#define KEY_R 114
#define KEY_P 112
#define KEY_Q 113
#define KEY_Z 122

#define NO_INPUT -1

#define SPEED 1000

#define POS_AMOUNT 7
#define FIGURE_SIZE 4

#define PATH_SCORE "high_score.txt"

#include "../../brick_game.h"

typedef enum {
  Tfigure,
  Ofigure,
  Lfigure,
  Ifigure,
  Sfigure,
  Zfigure,
  Jfigure,
} FigureType_t;

typedef struct {
  FigureType_t now;
  FigureType_t next;
} CurrentFigures_t;

typedef struct {
  int **form;
  int size;
  int x;
  int y;
  FigureType_t type;
} Figure_t;

typedef struct {
  int **field;
  CurrentFigures_t figures;
  Figure_t now;
  Figure_t next;
  State_t state;
  UserAction_t action;
  long long time;
  int score;
  int high_score;
  int level;
  int speed;
  int addSpeed;
  int pause;
} GameState_t;

/**
 * @brief Создает новую фигуру
 */
void createFigure(Figure_t *, int, int);

/**
 * @brief Генерирует набор фигур
 *
 * Набор: текущая и следующая фигуры.
 */
void spawnFigures(GameState_t *);

/**
 * @brief Генерирует типы фигур
 */
void figuresUpdate(GameState_t *);

/**
 * @brief Очищает текущую фигуру на игровом поле
 */
void cleanFigure(GameState_t *gs);

/**
 * @brief Обновляет данные для отрисовки игрового поля
 */
GameInfo_t updateCurrentState();

/**
 * @brief Обрабатывает пользовательский ввод
 *
 * Интерпретирует код ввода для обработки информации
 */
UserAction_t getAction();

/**
 * @brief Получает текущие данные игры
 */
GameState_t *getGame();

/**
 * @brief Получает текущее состояние игры
 */
State_t *getState();

/**
 * @brief Обрабатывает пользовательский ввод
 *
 * В зависимости от состояния игры вызывает нужную функцию.
 */
void userInput(UserAction_t);

/**
 * @brief Обновляет позицию фигуры
 *
 * В зависимости от пользовательского ввода вызывает нужную функцию.
 */
void moveFigure(GameState_t *);

/**
 * @brief Сдвигает фигуру на один "пиксель" влево
 */
void moveLeft(GameState_t *);

/**
 * @brief Сдвигает фигуру на один "пиксель" вправо
 */
void moveRight(GameState_t *);

/**
 * @brief Ускоряет падение фигуры
 */
void moveDown(GameState_t *);

/**
 * @brief Вращает фигуру вправо на 90 градусов
 */
void rotate(GameState_t *);

/**
 * @brief Вращает фигуру вправо на 90 градусов
 */
void rotate90(Figure_t, Figure_t *, int);

/**
 * @brief Сдвигает фигуру на один "пиксель" вниз
 */
void shift(GameState_t *);

/**
 * @brief Завершает игру
 *
 * Стирает и обнуляет данные игры.
 */
void finishGame(GameState_t *);

/**
 * @brief Инициализирует данные игры
 */
void gameInit(GameState_t *);

/**
 * @brief Возвращает текущее время в миллисекундах
 */
long long getTime();

/**
 * @brief Обрабатывает время начала и конца отсчета
 *
 * Если (время конца - время начала) равно скорости падения фигуры,
 * то состояние игры меняется на SHIFTING
 */
void timer(GameState_t *);

/**
 * @brief Добавляет текущую фигуру на игровое поле и закрепляет ее
 */
void updateGameField(GameState_t *);

/**
 * @brief Удаляет заполненные линии
 */
void deleteFullLines(GameState_t *);

/**
 * @brief Проверяет касание фигуры верхней границы поля
 */
bool checkTopBorder(GameState_t const *);

/**
 * @brief Проверяет касание фигуры левой границы поля
 * (или блока)
 */
bool checkLeftBorder(GameState_t const *);

/**
 * @brief Проверяет касание фигуры правой границы поля
 * (или блока)
 */
bool checkRightBorder(GameState_t const *);

/**
 * @brief Проверяет касание фигуры нижней границы поля
 * (или блока)
 */
bool checkBottomBorder(GameState_t const *);

/**
 * @brief Проверяет окружение фигуры для дальнейшего поворота
 */
bool checkRotateByBorder(GameState_t const *);

/**
 * @brief Генерирует новую фигуру
 */
void figureGenerate(Figure_t *, FigureType_t);

/**
 * @brief Очищает фигуру
 */
void freeFigure(Figure_t *);

/**
 * @brief Добавляет текущую фигуру на игровое поле
 */
void updateField(GameState_t *);

/**
 * @brief Подсчитывает очки
 *
 * При достижении новых 600 очков увеличивается уровень игрока на 1
 * и скорость падения фигуры
 */
void scoring(GameState_t *, int);

/**
 * @brief Подсчитывает очки
 *
 * В зависимости от количества заполненных линий добавляется
 * разное количество очков
 */
int getScore(int);

/**
 * @brief Обновляет максимальный счет игры
 */
void getHighScore(GameState_t *);

/**
 * @brief Записывает максимальный счет игры в данные текущей игры
 */
void updateHighScore(GameState_t *);

#endif