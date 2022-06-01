#include "readfile.h"
#include "partition.h"
#include "placement.h"
#include "random.h"

#ifndef _SIMULATE_ANNEALING_H_
#define _SIMULATE_ANNEALING_H_

extern double CUT_OFF_TIME;
extern double start_time;

typedef struct _SA_CONTENT_{
	Die top_die;
	Die bottom_die;
	vector <Tech_menu> TechMenu;
	vector <RawNet> rawnet;
	vector <int> PartitionResult;
	vector <Instance> InstanceArray;
	TopBottomCellArray ArrayInfo;
}SA_CONTENT;

unsigned long long int Cost(SA_CONTENT SA_contentPtr);
SA_CONTENT Move1(SA_CONTENT SA_contentPtr, int *moveFlag);
SA_CONTENT SimulateAnnealing(SA_CONTENT SA_contentPtr, double ANNEALING_TEMPERATURE, double TERMINATE_TEMPERATURE, int INNER_LOOP_TIMES, double ALPHA);
void StoreBBOX(SA_CONTENT SA_contentPtr, vector <Net> &NetArray);
void printBBOX(vector <Net> NetArray);
#endif