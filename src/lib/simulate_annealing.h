#ifndef _SIMULATE_ANNEALING_H_
#define _SIMULATE_ANNEALING_H_

#define ANNEALING_TEMPERATURE 	40000
#define TERMINATE_TEMPERATURE 	1000
#define INNER_LOOP_TIMES 		100
#define alpha 					0.8


int Cost(TopBottomCellArray ArrayInfo, Die top_die, Die bottom_die, vector <RawNet> rawnet, vector <Tech_menu> TechMenu, vector <int> PartitionResult, vector <Instance> InstanceArray);

#endif