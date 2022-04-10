#include <stdio.h>
#include <assert.h>
#include "../src/lib/readfile.h"

int main(){
	printf("start\n");
	FILE *input = fopen("../Benchmark/case2.txt", "r");
	char buffer[50];
	fscanf(input, "%s", buffer);
	printf("%s\n", buffer);

	Die *top_die = NULL;
	Die *bottom_die = NULL;
	hybrid_terminal *term = NULL;
	int NumTechnologies;
	Tech_menu **tech_menu = NULL;
	int NumInstances;
	InstanceNode *Instancearray = NULL;
	int NumNets;
	Net *Netarray = NULL;

	readfile(input,top_die,bottom_die,term,&NumTechnologies, tech_menu, &NumInstances, Instancearray, &NumNets, Netarray);
	return 0;
}