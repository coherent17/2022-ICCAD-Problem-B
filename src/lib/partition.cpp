#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "partition.h"
#include "readfile.h"

#define TEMP_BUFFER_SIZE 10

void OutputPartitionFormat(int NumNets, int NumInstances, vector <RawNet> NetArray){
	FILE *shmetisInput = fopen("Netlist.hgr", "w");
	fprintf(shmetisInput, "%d %d\n", NumNets, NumInstances);

	for(int i = 0; i < NumNets; i++){
		for(int j = 0; j < NetArray[i].numPins; j++){
			char buffer[TEMP_BUFFER_SIZE];
			memset(buffer, '\0', sizeof(buffer));
			strncpy(buffer,  &(NetArray[i].Connection[j].instName[1]), strlen(NetArray[i].Connection[j].instName)-1);
			fprintf(shmetisInput, "%s ", buffer);
		}
		fprintf(shmetisInput, "\n");
	}
	fclose(shmetisInput);
}

//using shmetis to do 2-way partition and redirect the output stream to log.txt
void PartitionInstance(){
	system("chmod +x src/hmetis/shmetis");
	system("./src/hmetis/shmetis Netlist.hgr 2 5 > log.txt");
	system("rm -rf log.txt");
}

void ReadPartitionResult(vector <Instance> &InstanceArray, int NumInstances){
	FILE *shmetisResult = fopen("Netlist.hgr.part.2", "r");
	for(int i = 0; i < NumInstances; i++){
		fscanf(shmetisResult, "%d", &(InstanceArray[i].whichDie));
	}
	fclose(shmetisResult);
	system("rm -rf Netlist.hgr Netlist.hgr.part.2");
}

void printPartitionResult(vector <Instance> InstanceArray, int NumInstances){
	for(int i = 0; i < NumInstances; i++){
		printf("%d \n", InstanceArray[i].whichDie);
	}
}