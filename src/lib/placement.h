#include "readfile.h"
#include "partition.h"

#ifndef _PLACEMENT_H_
#define _PLACEMENT_H_

#define EMPTY_STATE -1
#define BOUNDARY_INVALID -2
#define PLACEMENT_USED -3

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
void printPlacementState(Die current_die, bool PartitionAgain);
void OutputCellLocateState(TopBottomCellArray ArrayInfo, Die top_die, Die bottom_die, vector <RawNet> rawnet, vector <Tech_menu> TechMenu, vector <int> PartitionResult, vector <Instance> InstanceArray);
void printArrayInfo(TopBottomCellArray *ArrayInfo);
void HybridPlacement(Hybrid_terminal *terminal, Die top_die, vector <Net> &NetArray);
void outputAnswer(char *filename, TopBottomCellArray ArrayInfo, Die top_die, Die bottom_die, int NumTerminal, vector <Net> NetArray);
#endif