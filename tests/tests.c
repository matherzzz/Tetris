#include <check.h>

#include "../brick_game/tetris/inc/fsm.h"

START_TEST(init_1) {
  Tetris_t tetris;
  startGame(&tetris);
  ck_assert_int_eq(tetris.gameInfo.score, 0);
  endGame(&tetris);
}
END_TEST

START_TEST(init_2) {
  Tetris_t tetris;
  startGame(&tetris);
  ck_assert_int_eq(tetris.gameInfo.level, 0);
  endGame(&tetris);
}
END_TEST

START_TEST(init_3) {
  Tetris_t tetris;
  startGame(&tetris);
  ck_assert_int_eq(tetris.gameInfo.pause, 0);
  endGame(&tetris);
}
END_TEST

START_TEST(start_1) {
  Tetris_t tetris;
  startGame(&tetris);
  userInput(Start, &tetris);
  updateCurrentState(&tetris);
  ck_assert_int_eq(tetris.state, SPAWN);
  endGame(&tetris);
}
END_TEST

START_TEST(spawn_2) {
  Tetris_t tetris;
  startGame(&tetris);
  userInput(Start, &tetris);
  updateCurrentState(&tetris);
  updateCurrentState(&tetris);
  ck_assert_int_eq(tetris.state, MOVING);
  endGame(&tetris);
}
END_TEST

START_TEST(left_1) {
  Tetris_t tetris;
  startGame(&tetris);
  userInput(Start, &tetris);
  updateCurrentState(&tetris);
  updateCurrentState(&tetris);
  userInput(getSignal(KEY_LEFT), &tetris);
  updateCurrentState(&tetris);
  ck_assert_int_eq(tetris.state, SHIFTING);
  ck_assert_int_eq(tetris.gameInfo.pause, 0);
  endGame(&tetris);
}
END_TEST

START_TEST(right_1) {
  Tetris_t tetris;
  startGame(&tetris);
  userInput(Start, &tetris);
  updateCurrentState(&tetris);
  updateCurrentState(&tetris);
  userInput(Right, &tetris);
  updateCurrentState(&tetris);
  ck_assert_int_eq(tetris.state, SHIFTING);
  ck_assert_int_eq(tetris.gameInfo.pause, 0);
  endGame(&tetris);
}
END_TEST

START_TEST(right_2) {
  Tetris_t tetris;
  startGame(&tetris);
  userInput(Start, &tetris);
  updateCurrentState(&tetris);
  updateCurrentState(&tetris);
  userInput(getSignal(KEY_RIGHT), &tetris);
  updateCurrentState(&tetris);
  updateCurrentState(&tetris);
  ck_assert_int_eq(tetris.state, MOVING);
  ck_assert_int_eq(tetris.gameInfo.pause, 0);
  endGame(&tetris);
}
END_TEST

START_TEST(down_1) {
  Tetris_t tetris;
  startGame(&tetris);
  userInput(Start, &tetris);
  updateCurrentState(&tetris);
  updateCurrentState(&tetris);
  userInput(Down, &tetris);
  updateCurrentState(&tetris);
  ck_assert_int_eq(tetris.state, ATTACHING);
  ck_assert_int_eq(tetris.gameInfo.pause, 0);
  endGame(&tetris);
}
END_TEST

START_TEST(action_1) {
  Tetris_t tetris;
  startGame(&tetris);
  userInput(Start, &tetris);
  updateCurrentState(&tetris);
  updateCurrentState(&tetris);
  userInput(Down, &tetris);
  updateCurrentState(&tetris);
  updateCurrentState(&tetris);
  userInput(Action, &tetris);
  updateCurrentState(&tetris);
  ck_assert_int_eq(tetris.state, MOVING);
  ck_assert_int_eq(tetris.gameInfo.pause, 0);
  endGame(&tetris);
}
END_TEST

START_TEST(action_2) {
  Tetris_t tetris;
  startGame(&tetris);
  userInput(Start, &tetris);
  updateCurrentState(&tetris);
  updateCurrentState(&tetris);
  userInput(Down, &tetris);
  updateCurrentState(&tetris);
  updateCurrentState(&tetris);
  userInput(Action, &tetris);
  updateCurrentState(&tetris);
  updateCurrentState(&tetris);
  ck_assert_int_eq(tetris.state, SHIFTING);
  ck_assert_int_eq(tetris.gameInfo.pause, 0);
  endGame(&tetris);
}
END_TEST

START_TEST(pause_1) {
  Tetris_t tetris;
  startGame(&tetris);
  userInput(Start, &tetris);
  updateCurrentState(&tetris);
  updateCurrentState(&tetris);
  userInput(Down, &tetris);
  updateCurrentState(&tetris);
  updateCurrentState(&tetris);
  userInput(Action, &tetris);
  updateCurrentState(&tetris);
  userInput(Pause, &tetris);
  updateCurrentState(&tetris);
  userInput(NOSIG, &tetris);
  updateCurrentState(&tetris);
  ck_assert_int_eq(tetris.state, PAUSE);
  ck_assert_int_eq(tetris.gameInfo.pause, 1);
  endGame(&tetris);
}
END_TEST

START_TEST(terminate_1) {
  Tetris_t tetris;
  startGame(&tetris);
  userInput(Start, &tetris);
  updateCurrentState(&tetris);
  updateCurrentState(&tetris);
  userInput(Down, &tetris);
  updateCurrentState(&tetris);
  updateCurrentState(&tetris);
  userInput(Action, &tetris);
  updateCurrentState(&tetris);
  userInput(Pause, &tetris);
  updateCurrentState(&tetris);
  userInput(NOSIG, &tetris);
  updateCurrentState(&tetris);
  userInput(Terminate, &tetris);
  updateCurrentState(&tetris);
  ck_assert_int_eq(tetris.state, EXIT_STATE);
  ck_assert_int_eq(tetris.gameInfo.pause, 1);
  endGame(&tetris);
}
END_TEST

START_TEST(terminate_2) {
  Tetris_t tetris;
  startGame(&tetris);
  ;
  userInput(getSignal(ESCAPE), &tetris);
  updateCurrentState(&tetris);
  ck_assert_int_eq(tetris.state, EXIT_STATE);
  ck_assert_int_eq(tetris.gameInfo.pause, 0);
  endGame(&tetris);
}
END_TEST

START_TEST(nosig_1) {
  Tetris_t tetris;
  startGame(&tetris);
  ;
  userInput(89, &tetris);
  updateCurrentState(&tetris);
  ck_assert_int_eq(tetris.state, START);
  ck_assert_int_eq(tetris.gameInfo.pause, 0);
  endGame(&tetris);
}
END_TEST

START_TEST(gameover_1) {
  Tetris_t tetris;
  startGame(&tetris);
  userInput(Start, &tetris);
  updateCurrentState(&tetris);
  updateCurrentState(&tetris);
  userInput(Down, &tetris);
  updateCurrentState(&tetris);
  updateCurrentState(&tetris);
  userInput(Action, &tetris);
  updateCurrentState(&tetris);
  userInput(Pause, &tetris);
  updateCurrentState(&tetris);
  userInput(NOSIG, &tetris);
  updateCurrentState(&tetris);
  userInput(Pause, &tetris);
  updateCurrentState(&tetris);
  while (tetris.state != GAMEOVER) {
    userInput(Down, &tetris);
    updateCurrentState(&tetris);
  }
  ck_assert_int_eq(tetris.state, GAMEOVER);
  ck_assert_int_eq(tetris.gameInfo.pause, 0);
  endGame(&tetris);
}
END_TEST

START_TEST(gameover_2) {
  Tetris_t tetris;
  startGame(&tetris);
  userInput(Start, &tetris);
  updateCurrentState(&tetris);
  updateCurrentState(&tetris);
  userInput(Down, &tetris);
  updateCurrentState(&tetris);
  updateCurrentState(&tetris);
  userInput(Action, &tetris);
  updateCurrentState(&tetris);
  userInput(Pause, &tetris);
  updateCurrentState(&tetris);
  userInput(NOSIG, &tetris);
  updateCurrentState(&tetris);
  userInput(Pause, &tetris);
  updateCurrentState(&tetris);
  while (tetris.state != GAMEOVER) {
    userInput(Down, &tetris);
    updateCurrentState(&tetris);
  }
  updateCurrentState(&tetris);
  ck_assert_int_eq(tetris.state, GAMEOVER);
  ck_assert_int_eq(tetris.gameInfo.pause, 0);
  endGame(&tetris);
}
END_TEST

START_TEST(gameover_3) {
  Tetris_t tetris;
  startGame(&tetris);
  userInput(Start, &tetris);
  updateCurrentState(&tetris);
  updateCurrentState(&tetris);
  userInput(getSignal(KEY_DOWN), &tetris);
  updateCurrentState(&tetris);
  updateCurrentState(&tetris);
  userInput(getSignal(KEY_UP), &tetris);
  updateCurrentState(&tetris);
  userInput(Pause, &tetris);
  updateCurrentState(&tetris);
  userInput(NOSIG, &tetris);
  updateCurrentState(&tetris);
  userInput(getSignal(SPACE), &tetris);
  updateCurrentState(&tetris);
  while (tetris.state != GAMEOVER) {
    userInput(Down, &tetris);
    updateCurrentState(&tetris);
  }
  updateCurrentState(&tetris);
  userInput(getSignal(ENTER_KEY), &tetris);
  updateCurrentState(&tetris);
  ck_assert_int_eq(tetris.state, SPAWN);
  ck_assert_int_eq(tetris.gameInfo.pause, 0);
  endGame(&tetris);
}
END_TEST

Suite *tetris_suite() {
  Suite *s = suite_create("tetris_suite");
  TCase *tc = tcase_create("tetris_tc");

  tcase_add_test(tc, init_1);
  tcase_add_test(tc, init_2);
  tcase_add_test(tc, init_3);
  tcase_add_test(tc, start_1);
  tcase_add_test(tc, spawn_2);
  tcase_add_test(tc, left_1);
  tcase_add_test(tc, right_1);
  tcase_add_test(tc, right_2);
  tcase_add_test(tc, down_1);
  tcase_add_test(tc, action_1);
  tcase_add_test(tc, action_2);
  tcase_add_test(tc, pause_1);
  tcase_add_test(tc, terminate_1);
  tcase_add_test(tc, terminate_2);
  tcase_add_test(tc, nosig_1);
  tcase_add_test(tc, gameover_1);
  tcase_add_test(tc, gameover_2);
  tcase_add_test(tc, gameover_3);
  suite_add_tcase(s, tc);

  return s;
}

int main() {
  Suite *s = tetris_suite();
  SRunner *sr = srunner_create(s);
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_VERBOSE);
  int tf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return tf > 0;
}