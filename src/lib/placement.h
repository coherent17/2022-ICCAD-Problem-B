#include "readfile.h"
#include "partition.h"

#ifndef _PLACEMENT_H_
#define _PLACEMENT_H_

#define EMPTY_STATE -1

typedef struct _point{
	int x_cor;
	int y_cor;
	int comeFrom;
} point;

void InitializePlacement(Die *currentDie, TopBottomCellArray *ArrayInfo, int flag, bool *PartitionAgain);
void printPlacementState(Die current_die, bool PartitionAgain);
void OutputCellLocateState(TopBottomCellArray ArrayInfo, Die top_die, Die bottom_die, vector <RawNet> rawnet, vector <Tech_menu> TechMenu, vector <int> PartitionResult, vector <Instance> InstanceArray);

#endif