#include "readfile.h"
#include "partition.h"
#include "placement.h"
#include "random.h"

#ifndef _SIMULATE_ANNEALING_H_
#define _SIMULATE_ANNEALING_H_

#define ANNEALING_TEMPERATURE 	2400000000//24000
#define TERMINATE_TEMPERATURE 	0.0226//0.0001
#define INNER_LOOP_TIMES 		12000//160
#define ALPHA					0.1//0.95

typedef struct _SA_CONTENT_{
	Die top_die;
	Die bottom_die;
	vector <Tech_menu> TechMenu;
	vector <RawNet> rawnet;
	vector <int> PartitionResult;
	vector <Instance> InstanceArray;
	TopBottomCellArray ArrayInfo;
}SA_CONTENT;

int Cost(SA_CONTENT SA_contentPtr);
SA_CONTENT Move1(SA_CONTENT SA_contentPtr, int *moveFlag);
SA_CONTENT SimulateAnnealing(SA_CONTENT SA_contentPtr);

#endif