#include "unit_test.h"

START_TEST(getGameState_1) {
  GameState_t *gs = getGame();
  State_t *state = getState();

  ck_assert_int_eq(gs->state, START);
  ck_assert_int_eq(*state, START);
}
END_TEST

START_TEST(updateCurrentState_1) {
  GameState_t *gs = getGame();
  GameInfo_t gi = updateCurrentState();
  State_t *state = getState();

  ck_assert_ptr_eq(gs->field, gi.field);
  ck_assert_ptr_eq(gs->next.form, gi.next);

  ck_assert_int_eq(gs->speed, gi.speed);
  ck_assert_int_eq(gs->score, gi.score);
  ck_assert_int_eq(gs->high_score, gi.high_score);
  ck_assert_int_eq(gs->level, gi.level);
  ck_assert_int_eq(gs->pause, gi.pause);
  ck_assert_int_eq(gs->state, *state);
}
END_TEST

START_TEST(userInput_1) {
  GameState_t *gs = getGame();
  ck_assert_int_eq(gs->state, START);
  ck_assert_ptr_null(gs->field);

  userInput(Start);

  _ck_assert_int(gs->state, !=, START);
  ck_assert_ptr_nonnull(gs->field);
  finishGame(gs);
  gs->state = START;
}
END_TEST

START_TEST(userInput_2) {
  GameState_t *gs = getGame();

  userInput(Start);
  ck_assert_int_eq(gs->pause, 0);

  userInput(Pause);
  ck_assert_int_eq(gs->pause, 1);

  userInput(Pause);
  ck_assert_int_eq(gs->pause, 0);

  finishGame(gs);
  gs->state = START;
}
END_TEST

START_TEST(userInput_3) {
  GameState_t *gs = getGame();
  ck_assert_int_eq(gs->state, START);

  userInput(Start);

  userInput(Terminate);
  ck_assert_int_eq(gs->state, GAMEOVER);

  gs->state = START;
}
END_TEST

START_TEST(userInput_4) {
  GameState_t *gs = getGame();
  ck_assert_int_eq(gs->state, START);

  userInput(Start);
  int start = gs->now.x;
  userInput(Left);
  int end = gs->now.x;

  ck_assert_int_eq(start - end, 1);

  finishGame(gs);
  gs->state = START;
}
END_TEST

START_TEST(userInput_5) {
  GameState_t *gs = getGame();
  ck_assert_int_eq(gs->state, START);

  userInput(Start);
  int start = gs->now.x;
  userInput(Right);
  int end = gs->now.x;

  ck_assert_int_eq(start - end, -1);

  finishGame(gs);
  gs->state = START;
}
END_TEST

START_TEST(userInput_6) {
  GameState_t *gs = getGame();
  ck_assert_int_eq(gs->state, START);

  userInput(Start);
  ck_assert_int_eq(gs->speed, SPEED);
  userInput(Down);
  ck_assert_int_eq(gs->speed, 50);

  finishGame(gs);
  gs->state = START;
}
END_TEST

START_TEST(userInput_7) {
  GameState_t *gs = getGame();
  ck_assert_int_eq(gs->state, START);

  userInput(Start);
  if (gs->now.y == -1) shift(gs);

  int size = gs->now.size;
  int f1[FIGURE_SIZE][FIGURE_SIZE] = {{0}};
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      f1[j][size - 1 - i] = gs->now.form[i][j];
    }
  }

  userInput(Up);
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      ck_assert_int_eq(gs->now.form[i][j], f1[i][j]);
      ;
    }
  }

  finishGame(gs);
  gs->state = START;
}
END_TEST

START_TEST(userInput_8) {
  GameState_t *gs = getGame();
  ck_assert_int_eq(gs->state, START);

  userInput(Start);
  if (gs->now.y == -1) shift(gs);

  int size = gs->now.size;
  int f1[FIGURE_SIZE][FIGURE_SIZE] = {{0}};
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      f1[j][size - 1 - i] = gs->now.form[i][j];
    }
  }

  userInput(Action);
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      ck_assert_int_eq(gs->now.form[i][j], f1[i][j]);
      ;
    }
  }

  finishGame(gs);
  gs->state = START;
}
END_TEST

START_TEST(shifting_1) {
  GameState_t *gs = getGame();
  userInput(Start);
  int start = gs->now.y;
  gs->state = SHIFTING;
  userInput(NO_INPUT);
  int end = gs->now.y;

  ck_assert_int_eq(end - start, 1);

  finishGame(gs);
  gs->state = START;
}
END_TEST

START_TEST(attaching_1) {
  GameState_t *gs = getGame();
  userInput(Start);
  for (int i = 0; i < WIDTH; i++) {
    gs->field[HEIGHT - 1][i] = 1;
  }
  gs->state = ATTACHING;
  userInput(NO_INPUT);

  for (int i = 0; i < WIDTH; i++) {
    ck_assert_int_eq(gs->field[HEIGHT - 1][i], 0);
  }

  finishGame(gs);
  gs->state = START;
}
END_TEST

START_TEST(getAction_1) {
  UserAction_t a = getAction();

  ck_assert_int_eq(a, (unsigned)NO_INPUT);
}
END_TEST

START_TEST(updateHighScore_1) {
  GameState_t *gs = getGame();
  gs->high_score = 0;
  FILE *f = fopen(PATH_SCORE, "wt");
  if (f) {
    fprintf(f, "1234");
    fclose(f);
  }
  updateHighScore(gs);

  ck_assert_int_eq(gs->high_score, 1234);
}
END_TEST

Suite *tetris_suite() {
  Suite *s = suite_create("tetris_suite");
  TCase *tc = tcase_create("tetris_tc");

  tcase_add_test(tc, getGameState_1);
  tcase_add_test(tc, updateCurrentState_1);
  tcase_add_test(tc, userInput_1);
  tcase_add_test(tc, userInput_2);
  tcase_add_test(tc, userInput_3);
  tcase_add_test(tc, userInput_4);
  tcase_add_test(tc, userInput_5);
  tcase_add_test(tc, userInput_6);
  tcase_add_test(tc, userInput_7);
  tcase_add_test(tc, userInput_8);
  tcase_add_test(tc, shifting_1);
  tcase_add_test(tc, attaching_1);
  tcase_add_test(tc, getAction_1);
  tcase_add_test(tc, updateHighScore_1);

  suite_add_tcase(s, tc);

  return s;
}

int main() {
  Suite *s = tetris_suite();
  SRunner *sr = srunner_create(s);
  int tf = 0;

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_VERBOSE);
  tf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return tf > 0;
}