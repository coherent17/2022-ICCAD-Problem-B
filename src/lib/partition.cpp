#include <stdio.h>
#include <stdlib.h>
#include "partition.h"
#include "readfile.h"

void OutputPartitionFormat(int NumNets, int NumInstances, Net *NetArray){
	FILE *shmetisInput = fopen("Netlist.hgr", "w");
	fprintf(shmetisInput, "%d %d\n", NumNets, NumInstances);

	for(int i = 0; i < NumNets; i++){
		for(int j = 0; j < NetArray[i].numPins; j++){
			fprintf(shmetisInput, "%s ", NetArray[i].Connection[j].instName);
		}
		fprintf(shmetisInput, "\n");
	}
	fclose(shmetisInput);
}

void PartitionInstance(){
	system("chmod +x src/hmetis/shmetis");
	system("./src/hmetis/shmetis Netlist.hgr 2 5");
}