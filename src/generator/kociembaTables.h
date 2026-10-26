#ifndef KOCIEMBA_TABLES_H
#define KOCIEMBA_TABLES_H

#include "solver/kociemba/moveController.h"

void generatePhase1Tables(MoveController& moveController, int maxDepth = 255);
void generatePhase2Tables(MoveController& moveController, int maxDepth = 255);

#endif
