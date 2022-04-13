#include <stdio.h>
#include <assert.h>
#include "../src/lib/readfile.h"

int main(){
	FILE *input = fopen("Benchmark/case1.txt", "r");
	assert(input);

	Die top_die, bottom_die;
	Hybrid_terminal terminal;
	int NumTechnologies;
	Tech_menu *TechMenu = NULL;
	int NumInstances;
	Instance *InstanceArray = NULL;
	int NumNets;
	Net *NetArray = NULL;

	readDieInfo(input, &top_die, &bottom_die);
	printDieInfo(top_die, bottom_die);
	readHybridTerminalInfo(input, &terminal);
	printHybridTerminalInfo(terminal);
	readTechnologyInfo(input, &NumTechnologies, &TechMenu);
	printTechnologyInfo(NumTechnologies, TechMenu);
	readInstanceInfo(input, &NumInstances, &InstanceArray);
	printInstanceInfo(NumInstances, InstanceArray);
	readNetInfo(input, &NumNets, &NetArray);
	printNetInfo(NumNets, NetArray);
	

	freeTech_menu(NumTechnologies, &TechMenu);
	freeInstanceArray(NumInstances, &InstanceArray);
	freeNetArray(NumNets, &NetArray);
	fclose(input);
	return 0;
}