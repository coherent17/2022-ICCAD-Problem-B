#include "readfile.h"
#include "partition.h"

#ifndef _PLACEMENT_H_
#define _PLACEMENT_H_

#define EMPTY_STATE -1

void InitializePlacement(Die *currentDie, TopBottomCellArray ArrayInfo, int flag);
void printPlacementState(Die current_die);

#endif