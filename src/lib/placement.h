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

typedef struct _bbox{
	int x_min;
	int x_max;
	int y_min;
	int y_max;
} bbox;

void InitializePlacement(Die *currentDie, TopBottomCellArray *ArrayInfo, int flag, bool *PartitionAgain);
bool isValidPlacement(vector <vector <int>> PlacementState, int row, int left_edge, int right_edge);
void printPlacementState(Die current_die, bool PartitionAgain);
void OutputCellLocateState(TopBottomCellArray ArrayInfo, Die top_die, Die bottom_die, vector <RawNet> rawnet, vector <Tech_menu> TechMenu, vector <int> PartitionResult, vector <Instance> InstanceArray);
void printArrayInfo(TopBottomCellArray *ArrayInfo);
#endif