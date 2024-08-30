#include "../inc/fsm.h"

static void onStartState(Tetris_t *tetris) {
  switch (tetris->user_action) {
    case Start:
      tetris->gameInfo.level = 1;
      tetris->state = SPAWN;
      break;
    case Terminate:
      tetris->state = EXIT_STATE;
      break;
    default:
      tetris->state = START;
      break;
  }
}

static void onSpawnState(Tetris_t *tetris) {
  tetris->figure.type =
      tetris->figure.type_next == 0 ? rand() % 7 + 1 : tetris->figure.type_next;
  tetris->figure.type_next = rand() % 7 + 1;
  while (tetris->figure.type_next == tetris->figure.type)
    tetris->figure.type_next = rand() % 7 + 1;
  generateFigure(&tetris->figure);
  addToNext(&tetris->gameInfo, tetris->figure.type_next);
  tetris->state = MOVING;
}

static void onMovingState(Tetris_t *tetris) {
  switch (tetris->user_action) {
    case Action:
      rotate(tetris);
      break;
    case Down:
      moveDown(tetris, FALSE);
      tetris->state = ATTACHING;
      break;
    case Right:
      moveRight(tetris);
      break;
    case Left:
      moveLeft(tetris);
      break;
    case Pause:
      tetris->state = PAUSE;
      break;
    case Terminate:
      tetris->state = EXIT_STATE;
      break;
    default:
      break;
  }
  if (tetris->state != EXIT_STATE && tetris->state != PAUSE &&
      tetris->state != ATTACHING)
    tetris->state = SHIFTING;
}

static void onShiftingState(Tetris_t *tetris) {
  tetris->state = moveDown(tetris, TRUE) ? ATTACHING : MOVING;
}

static void onAttachingState(Tetris_t *tetris) {
  for (int i = 0; tetris->state != GAMEOVER && i < 4; i++)
    if (tetris->figure.bricks[i].y <= 0) tetris->state = GAMEOVER;
  if (tetris->state != GAMEOVER) tetris->state = SPAWN;
  removeLines(&tetris->gameInfo);
}

static void onGameOverState(Tetris_t *tetris) {
  resetGame(&tetris->gameInfo);
  tetris->gameInfo.level = -1;
  switch (tetris->user_action) {
    case Start:
      tetris->gameInfo.level = 1;
      tetris->state = SPAWN;
      break;
    case Terminate:
      tetris->state = EXIT_STATE;
      break;
    default:
      tetris->state = GAMEOVER;
      break;
  }
}

static void onPauseState(Tetris_t *tetris) {
  tetris->gameInfo.pause = TRUE;
  if (tetris->user_action == Pause) {
    tetris->state = MOVING;
    tetris->gameInfo.pause = FALSE;
  } else if (tetris->user_action == Terminate) {
    tetris->state = EXIT_STATE;
  }
}

UserAction_t getSignal(int user_input) {
  UserAction_t sig = NOSIG;
  if (user_input == KEY_UP) {
    sig = Action;
  } else if (user_input == KEY_DOWN) {
    sig = Down;
  } else if (user_input == KEY_LEFT) {
    sig = Left;
  } else if (user_input == KEY_RIGHT) {
    sig = Right;
  } else if (user_input == ESCAPE) {
    sig = Terminate;
  } else if (user_input == ENTER_KEY) {
    sig = Start;
  } else if (user_input == SPACE) {
    sig = Pause;
  }
  return sig;
}

void userInput(UserAction_t action, Tetris_t *tetris) {
  tetris->user_action = action;
}

GameInfo_t updateCurrentState(Tetris_t *tetris) {
  switch (tetris->state) {
    case START:
      onStartState(tetris);
      break;
    case SPAWN:
      onSpawnState(tetris);
      break;
    case MOVING:
      onMovingState(tetris);
      break;
    case SHIFTING:
      onShiftingState(tetris);
      break;
    case ATTACHING:
      onAttachingState(tetris);
      break;
    case GAMEOVER:
      onGameOverState(tetris);
      break;
    case PAUSE:
      onPauseState(tetris);
      break;
    default:
      break;
  }
  return tetris->gameInfo;
}