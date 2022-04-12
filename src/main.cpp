#include <stdio.h>
#include <assert.h>
#include "../src/lib/readfile.h"

int main(){
	FILE *input = fopen("Benchmark/case1.txt", "r");
	assert(input);

	Die top_die, bottom_die;
	Hybrid_terminal terminal;
	int NumTechnologies;
	Tech_menu *tech_menu = NULL;;

	/*
	int NumInstances;
	InstanceNode *Instancearray = NULL;
	int NumNets;
	Net *Netarray = NULL;
	*/

	readDieInfo(input, &top_die, &bottom_die);
	printDieInfo(&top_die, &bottom_die);
	readHybridTerminalInfo(input, &terminal);
	printHybridTerminalInfo(&terminal);
	readTechnology(input, &NumTechnologies, &tech_menu);
	printf("test!%d\n", tech_menu[1].libcell_count);
	
	return 0;
}