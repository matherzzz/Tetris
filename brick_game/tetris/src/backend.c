#include "../inc/backend.h"

static void addToField(Tetris_t *tetris) {
  for (int i = 0; i < 4; i++) {
    int x = tetris->figure.bricks[i].x;
    int y = tetris->figure.bricks[i].y;
    if (y >= 0) tetris->gameInfo.field[y][x] = 1;
  }
}

static void removeFromField(Tetris_t *tetris) {
  for (int i = 0; i < 4; i++) {
    int x = tetris->figure.bricks[i].x;
    int y = tetris->figure.bricks[i].y;
    if (y >= 0) tetris->gameInfo.field[y][x] = 0;
  }
}

static int isFull(GameInfo_t *gameInfo, int row) {
  int full = TRUE;
  for (int i = 0; full && i < 10; i++)
    if (!gameInfo->field[row][i]) full = FALSE;
  return full;
}

static int createField(GameInfo_t *gameInfo) {
  int code = OK;
  gameInfo->field = calloc(20, sizeof(int *));
  gameInfo->next = calloc(2, sizeof(int *));
  if (!gameInfo->field || !gameInfo->next) code = MEMORY_ERR;
  for (int i = 0; !code && i < 20; i++) {
    gameInfo->field[i] = calloc(10, sizeof(int));
    if (!gameInfo->field[i]) code = MEMORY_ERR;
  }
  for (int i = 0; !code && i < 2; i++) {
    gameInfo->next[i] = calloc(4, sizeof(int));
    if (!gameInfo->next[i]) code = MEMORY_ERR;
  }
  return code;
}

static int readSave() {
  FILE *file = fopen("./.save", "r");
  int result = 0;
  if (file) {
    char score[10];
    fgets(score, 10, file);
    result = atoi(score);
    fclose(file);
  }
  return result;
}

static void writeSave(int hi_score) {
  FILE *file = fopen("./.save", "w");
  if (file) {
    char score[10];
    sprintf(score, "%d", hi_score);
    fputs(score, file);
    fclose(file);
  }
}

void resetGame(GameInfo_t *gameInfo) {
  gameInfo->level = 1;
  gameInfo->speed = 1;
  gameInfo->score = 0;
  for (int i = 0; i < 20; i++)
    for (int j = 0; j < 10; j++) gameInfo->field[i][j] = 0;
}

int startGame(Tetris_t *tetris) {
  tetris->state = START;
  for (int i = 0; i < 4; i++) {
    tetris->figure.bricks[i].x = 0;
    tetris->figure.bricks[i].y = 0;
  }
  tetris->figure.type = 0;
  tetris->figure.type_next = 0;
  int code = createField(&tetris->gameInfo);
  if (!code) {
    tetris->gameInfo.score = 0;
    tetris->gameInfo.high_score = readSave();  //
    tetris->gameInfo.level = 0;
    tetris->gameInfo.pause = FALSE;
    tetris->gameInfo.speed = 1;
  }
  return code;
}

void endGame(Tetris_t *tetris) {
  if (tetris->gameInfo.field)
    for (size_t i = 0; i < 20; i++) free(tetris->gameInfo.field[i]);
  free(tetris->gameInfo.field);
  if (tetris->gameInfo.next)
    for (size_t i = 0; i < 2; i++) free(tetris->gameInfo.next[i]);
  free(tetris->gameInfo.next);
}

static int checkCollideY(Tetris_t *tetris) {
  int code = FALSE;
  for (int i = 0; i < 4; i++) {
    int x = tetris->figure.bricks[i].x;
    int y = tetris->figure.bricks[i].y;
    if (y >= 0 && y <= 19 && (y == 19 || tetris->gameInfo.field[y + 1][x]))
      code = TRUE;
  }
  return code;
}

static int checkCollideX(Tetris_t *tetris, int flag_left) {
  int code = FALSE;
  for (int i = 0; !code && i < 4; i++) {
    int x = tetris->figure.bricks[i].x;
    int y = tetris->figure.bricks[i].y;
    if (!flag_left && (x == 9 || (y >= 0 && tetris->gameInfo.field[y][x + 1])))
      code = TRUE;
    if (flag_left && (x == 0 || (y >= 0 && tetris->gameInfo.field[y][x - 1])))
      code = TRUE;
  }
  return code;
}

void removeLines(GameInfo_t *gameInfo) {
  int count_lines = 0;
  static int goal = 600;
  for (int i = 0; i < 20; i++) {
    if (!isFull(gameInfo, i)) continue;
    count_lines++;
    for (int j = i; j > 0; j--)
      for (int k = 0; k < 10; k++)
        gameInfo->field[j][k] = gameInfo->field[j - 1][k];
    for (int j = 0; j < 10; j++) gameInfo->field[0][j] = 0;
  }
  if (count_lines == 1) gameInfo->score += 100;
  if (count_lines == 2) gameInfo->score += 300;
  if (count_lines == 3) gameInfo->score += 700;
  if (count_lines == 4) gameInfo->score += 1500;
  if (gameInfo->score > gameInfo->high_score) {
    gameInfo->high_score = gameInfo->score;
    writeSave(gameInfo->high_score);
  }
  while (count_lines > 0 && gameInfo->score >= goal && gameInfo->speed < 10) {
    goal += 600;
    gameInfo->speed++;
    gameInfo->level++;
  }
}

void generateFigure(Figure_t *figure) {
  int type = figure->type;
  if (type == 1 || type == 2 || type == 3) {
    for (int i = 0; i < 3; i++) {
      figure->bricks[i].x = i + 3;
      figure->bricks[i].y = -1;
    }
    figure->bricks[3].x = type + 2;
    figure->bricks[3].y = -2;
  } else if (type == 4) {
    for (int i = 0; i < 2; i++) {
      figure->bricks[i].x = i + 4;
      figure->bricks[i].y = -1;
      figure->bricks[i + 2].x = i + 4;
      figure->bricks[i + 2].y = -2;
    }
  } else if (type == 5) {
    for (int i = 0; i < 4; i++) {
      figure->bricks[i].x = i + 3;
      figure->bricks[i].y = -1;
    }
  } else if (type == 6) {
    for (int i = 0; i < 2; i++) {
      figure->bricks[i].x = i + 3;
      figure->bricks[i].y = -1;
      figure->bricks[i + 2].x = i + 4;
      figure->bricks[i + 2].y = -2;
    }
  } else if (type == 7) {
    for (int i = 0; i < 2; i++) {
      figure->bricks[i].x = i + 3;
      figure->bricks[i].y = -2;
      figure->bricks[i + 2].x = i + 4;
      figure->bricks[i + 2].y = -1;
    }
  }
}

void rotate(Tetris_t *tetris) {
  removeFromField(tetris);
  Brick_t prev_coords[4];
  for (int i = 0; i < 4; i++) prev_coords[i] = tetris->figure.bricks[i];
  int flag_collide = FALSE;
  Brick_t center;
  center.x = tetris->figure.bricks[1].x;
  center.y = tetris->figure.bricks[1].y;
  for (int i = 0; tetris->figure.type != 4 && !flag_collide && i < 4; i++) {
    Brick_t current_coords = tetris->figure.bricks[i];
    Brick_t new_coords = {0};
    new_coords.x = center.x - current_coords.y + center.y;
    new_coords.y = center.y + current_coords.x - center.x;
    if (new_coords.x < 0 || new_coords.x > 9 || new_coords.y < 0 ||
        new_coords.y > 19 || tetris->gameInfo.field[new_coords.x][new_coords.y])
      flag_collide = TRUE;
    if (!flag_collide) tetris->figure.bricks[i] = new_coords;
  }
  if (flag_collide)
    for (int i = 0; i < 4; i++) tetris->figure.bricks[i] = prev_coords[i];
  addToField(tetris);
}

int moveDown(Tetris_t *tetris, int flag_speed) {
  removeFromField(tetris);
  static int counter = 0;
  int collide = FALSE;
  if (flag_speed) {
    counter++;
    collide = checkCollideY(tetris);
    for (int i = 0; !collide && i < 4; i++)
      if (counter == 11 - tetris->gameInfo.speed) tetris->figure.bricks[i].y++;
    if (counter >= 11 - tetris->gameInfo.speed) counter = 0;
  } else {
    while (!checkCollideY(tetris))
      for (int i = 0; i < 4; i++) tetris->figure.bricks[i].y++;
    collide = TRUE;
  }
  addToField(tetris);
  return collide;
}

void moveRight(Tetris_t *tetris) {
  removeFromField(tetris);
  int collide = checkCollideX(tetris, FALSE);
  for (int i = 0; !collide && i < 4; i++) tetris->figure.bricks[i].x++;
  addToField(tetris);
}

void moveLeft(Tetris_t *tetris) {
  removeFromField(tetris);
  int collide = checkCollideX(tetris, TRUE);
  for (int i = 0; !collide && i < 4; i++) tetris->figure.bricks[i].x--;
  addToField(tetris);
}

void addToNext(GameInfo_t *gameInfo, int type) {
  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 4; j++) gameInfo->next[i][j] = 0;
  if (type == 1 || type == 2 || type == 3) {
    for (int i = 1; i < 4; i++) gameInfo->next[1][i] = 1;
    gameInfo->next[0][type] = 1;
  } else if (type == 4) {
    for (int i = 1; i < 3; i++) {
      gameInfo->next[0][i] = 1;
      gameInfo->next[1][i] = 1;
    }
  } else if (type == 5) {
    for (int i = 0; i < 4; i++) gameInfo->next[0][i] = 1;
  } else if (type == 6) {
    for (int i = 1; i < 3; i++) {
      gameInfo->next[0][i + 1] = 1;
      gameInfo->next[1][i] = 1;
    }
  } else if (type == 7) {
    for (int i = 1; i < 3; i++) {
      gameInfo->next[0][i] = 1;
      gameInfo->next[1][i + 1] = 1;
    }
  }
}