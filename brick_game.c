#include "brick_game/tetris/inc/fsm.h"
#include "gui/cli/frontend.h"

void game_loop() {
  srand(time(NULL));
  Tetris_t tetris;
  int break_flag = !startGame(&tetris);
  int signal = 0;
  while (break_flag) {
    if (tetris.state == EXIT_STATE) break_flag = FALSE;
    userInput(getSignal(signal), &tetris);
    printGame(updateCurrentState(&tetris));
    if (tetris.state == PAUSE || tetris.state == MOVING ||
        tetris.state == START || tetris.state == GAMEOVER)
      signal = getch();
  }
  endGame(&tetris);
}

int main() {
  WIN_INIT(0);
  game_loop();
  endwin();
  return 0;
}