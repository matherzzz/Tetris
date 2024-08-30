#ifndef OBJECTS_H
#define OBJECTS_H

#include <stdlib.h>

typedef enum {
  START = 0,
  SPAWN,
  MOVING,
  PAUSE,
  SHIFTING,
  ATTACHING,
  GAMEOVER,
  EXIT_STATE,
  FILE_ERROR_STATE
} State_t;

typedef struct {
  int x;
  int y;
} Brick_t;

typedef struct {
  int type;
  int type_next;
  Brick_t bricks[4];
} Figure_t;

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

typedef struct {
  State_t state;
  Figure_t figure;
  UserAction_t user_action;
  GameInfo_t gameInfo;
} Tetris_t;

#endif  // OBJECTS_H