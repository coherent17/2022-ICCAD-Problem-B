#include "readfile.h"
#include "partition.h"

#ifndef _PLACEMENT_H_
#define _PLACEMENT_H_

#define EMPTY_STATE -1

void InitializePlacement(Die *currentDie, TopBottomCellArray *ArrayInfo, int flag, bool *PartitionAgain);
void printPlacementState(Die current_die, bool PartitionAgain);
void OutputCellLocateState(TopBottomCellArray ArrayInfo, Die top_die, Die bottom_die);

#endif