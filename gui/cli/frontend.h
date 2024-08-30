#ifndef FRONTEND_H
#define FRONTEND_H

#include <curses.h>

#include "../../brick_game/tetris/inc/objects.h"

#define WIN_INIT(time)    \
  {                       \
    initscr();            \
    noecho();             \
    keypad(stdscr, TRUE); \
    curs_set(0);          \
    timeout(time);        \
  }

#define BOARDS_BEGIN 7
#define MVADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), c)

void printGame(GameInfo_t gameInfo);

#endif  // FRONTEND_H