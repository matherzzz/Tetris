#ifndef BACKEND_H
#define BACKEND_H

#include <curses.h>

#include "defines.h"
#include "objects.h"

int startGame(Tetris_t *tetris);
void endGame(Tetris_t *tetris);
void resetGame(GameInfo_t *gameInfo);
void removeLines(GameInfo_t *gameInfo);
void generateFigure(Figure_t *figure);
void moveLeft(Tetris_t *tetris);
void moveRight(Tetris_t *tetris);
int moveDown(Tetris_t *tetris, int flag_speed);
void rotate(Tetris_t *tetris);
void addToNext(GameInfo_t *gameInfo, int type);

#endif  // BACKEND_H