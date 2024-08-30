#ifndef FSM_H
#define FSM_H

#include <time.h>

#include "../inc/backend.h"
#include "defines.h"
#include "objects.h"

UserAction_t getSignal(int user_input);
void userInput(UserAction_t action, Tetris_t *tetris);
GameInfo_t updateCurrentState(Tetris_t *tetris);

#endif