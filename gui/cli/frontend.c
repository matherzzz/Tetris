#include "frontend.h"

static void printBlock(int y, int x, int sym) {
  MVADDCH(y * 2 + 1, x * 3 + 1, sym);
  MVADDCH(y * 2 + 1, x * 3 + 2, sym);
  MVADDCH(y * 2 + 1, x * 3 + 3, sym);
  MVADDCH(y * 2 + 2, x * 3 + 1, sym);
  MVADDCH(y * 2 + 2, x * 3 + 2, sym);
  MVADDCH(y * 2 + 2, x * 3 + 3, sym);
}

static void printField(int **field) {
  for (int i = 0; i < 20; i++)
    for (int j = 0; j < 10; j++)
      field[i][j] ? printBlock(i, j, ACS_DIAMOND) : printBlock(i, j, ' ');
}

static void printStats(GameInfo_t gameInfo) {
  mvprintw(2 + BOARDS_BEGIN, 42 + BOARDS_BEGIN, "SCORE");
  mvprintw(4 + BOARDS_BEGIN, 40 + BOARDS_BEGIN, "%07d", gameInfo.score);
  mvprintw(6 + BOARDS_BEGIN, 39 + BOARDS_BEGIN, "HI-SCORE");
  mvprintw(8 + BOARDS_BEGIN, 40 + BOARDS_BEGIN, "%07d", gameInfo.high_score);
  mvprintw(10 + BOARDS_BEGIN, 39 + BOARDS_BEGIN, "LEVEL %d",
           gameInfo.level > 0 ? gameInfo.level : 1);
  if (gameInfo.next) mvprintw(12 + BOARDS_BEGIN, 43 + BOARDS_BEGIN, "NEXT");
  for (int i = 0; gameInfo.next && i < 2; i++)
    for (int j = 0; j < 4; j++) {
      if (gameInfo.next[i][j]) {
        printBlock(7 + i, 11 + j, ACS_DIAMOND);
      } else {
        printBlock(7 + i, 11 + j, ' ');
      }
    }
}

static void printLabelTetris() {
  int shift = 14;
  for (int i = 0; i < 5; i++) mvaddch(1, shift + i, ACS_BLOCK);
  for (int i = 0; i < 4; i++) mvaddch(2 + i, shift + 2, ACS_BLOCK);

  for (int i = 6; i < 11; i++) {
    mvaddch(1, shift + i, ACS_BLOCK);
    mvaddch(3, shift + i, ACS_BLOCK);
    mvaddch(5, shift + i, ACS_BLOCK);
  }
  for (int i = 0; i < 4; i++) mvaddch(2 + i, shift + 6, ACS_BLOCK);

  for (int i = 12; i < 17; i++) mvaddch(1, shift + i, ACS_BLOCK);
  for (int i = 0; i < 4; i++) mvaddch(2 + i, shift + 14, ACS_BLOCK);

  for (int i = 18; i < 23; i++) mvaddch(1, shift + i, ACS_BLOCK);
  for (int i = 19; i < 22; i++) mvaddch(3, shift + i, ACS_BLOCK);
  for (int i = 0; i < 4; i++) mvaddch(2 + i, shift + 18, ACS_BLOCK);
  for (int i = 0; i < 2; i++) mvaddch(2 + i, shift + 22, ACS_BLOCK);

  for (int i = 0; i < 5; i++) mvaddch(1 + i, shift + 24, ACS_BLOCK);
  for (int i = 0; i < 5; i++) mvaddch(1 + i, shift + 28, ACS_BLOCK);
  for (int i = 0; i < 3; i++) mvaddch(2 + i, shift + 27 - i, ACS_BLOCK);

  for (int i = 1; i < 5; i++) mvaddch(1 + i, shift + 30, ACS_BLOCK);
  for (int i = 30; i < 35; i++) mvaddch(1, shift + i, ACS_BLOCK);
  for (int i = 30; i < 35; i++) mvaddch(5, shift + i, ACS_BLOCK);
}

static void printOverlay() {
  printLabelTetris();
  int size_x = 31;
  int size_scoreboard = 17;
  int size_y = 41;
  for (int i = 1; i < size_x + size_scoreboard; i++) MVADDCH(0, i, ACS_HLINE);
  for (int i = 1; i < size_y; i++) {
    MVADDCH(i, 0, ACS_VLINE);
    MVADDCH(i, size_x, ACS_VLINE);
    MVADDCH(i, size_x + size_scoreboard, ACS_VLINE);
  }
  MVADDCH(0, 0, ACS_ULCORNER);
  MVADDCH(0, size_x + size_scoreboard, ACS_URCORNER);
  MVADDCH(size_y, 0, ACS_LLCORNER);
  MVADDCH(size_y, size_x + size_scoreboard, ACS_LRCORNER);
  for (int i = 0 + 1; i < size_x + size_scoreboard; i++)
    MVADDCH(size_y, i, ACS_HLINE);

  mvprintw(2 + BOARDS_BEGIN, 42 + BOARDS_BEGIN, "SCORE");
  mvprintw(6 + BOARDS_BEGIN, 39 + BOARDS_BEGIN, "HI-SCORE");
  mvprintw(10 + BOARDS_BEGIN, 39 + BOARDS_BEGIN, "LEVEL");
  mvprintw(12 + BOARDS_BEGIN, 43 + BOARDS_BEGIN, "NEXT");
}

static void printPause(int sym) {
  int size_x = 31;
  for (int i = 0; i < 4; i++) {
    MVADDCH(25 + i, size_x + 7, sym);
    MVADDCH(25 + i, size_x + 10, sym);
  }
}

void printGame(GameInfo_t gameInfo) {
  printOverlay();
  if (!gameInfo.pause) {
    printField(gameInfo.field);
    printPause(' ');
    napms(25);
  } else {
    printPause('#');
    napms(-1);
  }
  if (gameInfo.level < 1) {
    if (gameInfo.level == -1)
      mvprintw(20 + BOARDS_BEGIN, 12 + BOARDS_BEGIN, "GAME OVER");
    mvprintw(35 + BOARDS_BEGIN, 34 + BOARDS_BEGIN, "         ");
    mvprintw(37 + BOARDS_BEGIN, 34 + BOARDS_BEGIN, "Start: Enter");
  } else {
    mvprintw(35 + BOARDS_BEGIN, 34 + BOARDS_BEGIN, "Rotate: ^");
    mvprintw(37 + BOARDS_BEGIN, 34 + BOARDS_BEGIN, "Pause: Space");
  }
  mvprintw(39 + BOARDS_BEGIN, 34 + BOARDS_BEGIN, "Exit: Escape");
  printStats(gameInfo);
}