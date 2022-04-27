#include <stdio.h>
#include <assert.h>
#include "../src/lib/readfile.h"
#include "../src/lib/partition.h"

int main(int argc, char *argv[]){
	char *inputName = *(argv + 1);
	FILE *input = fopen(inputName, "r");
	assert(input);

	Die top_die, bottom_die;
	Hybrid_terminal terminal;
	int NumTechnologies;
	Tech_menu *TechMenu = NULL;
	int NumInstances;
	Instance *InstanceArray = NULL;
	int NumNets;
	Net *NetArray = NULL;

	
	realAllInfo(input, &NumTechnologies, &TechMenu, &top_die, &bottom_die, &terminal, &NumInstances, &InstanceArray, &NumNets, &NetArray);
	//printTechnologyInfo(NumTechnologies, TechMenu);
	//printDieInfo(top_die, bottom_die);
	//printHybridTerminalInfo(terminal);
	//printInstanceInfo(NumInstances, InstanceArray);
	//printNetInfo(NumNets, NetArray);
	
	OutputPartitionFormat(NumNets, NumInstances, NetArray);
	PartitionInstance();

	//read the shmetis partition result
	ReadPartitionResult(InstanceArray, NumInstances);
	printPartitionResult(InstanceArray, NumInstances);

	freeTech_menu(NumTechnologies, &TechMenu);
	freeInstanceArray(NumInstances, &InstanceArray);
	freeNetArray(NumNets, &NetArray);
	fclose(input);
	return 0;
}