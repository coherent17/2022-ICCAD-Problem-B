#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
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

void ReadPartitionResult(TopBottomCellArray *ArrayInfo, int NumInstances, vector <int> &PartitionResult){
	FILE *shmetisResult = fopen("Netlist.hgr.part.2", "r");
	int BottomCellCount = 0;
	int TopCellCount = 0;
	for(int i = 0; i < NumInstances; i++){
		int currentPartitionResult;
		fscanf(shmetisResult, "%d", &(currentPartitionResult));
		if(currentPartitionResult == 0){
			BottomCellCount++;
			Cell temp;
			temp.WhichDie = 0;
			temp.cellID = i;
			temp.netsNumber = 0;
			(*ArrayInfo).BottomCellArray.emplace_back(temp);
			PartitionResult.emplace_back(0);
		}
		else if(currentPartitionResult == 1){
			TopCellCount++;
			Cell temp;
			temp.WhichDie = 1;
			temp.cellID = i;
			temp.netsNumber = 0;
			(*ArrayInfo).TopCellArray.emplace_back(temp);
			PartitionResult.emplace_back(1);
		}
		assert(currentPartitionResult != 0 || currentPartitionResult != 1);
	}
	(*ArrayInfo).BottomCellNumber = BottomCellCount;
	(*ArrayInfo).TopCellNumber = TopCellCount;
	fclose(shmetisResult);
	system("rm -rf Netlist.hgr");
}

void printPartitionResult(TopBottomCellArray ArrayInfo, vector <Instance> InstanceArray, vector <int> PartitionResult){
	for(int i = 0; i < (int)PartitionResult.size(); i++){
		printf("%d ", PartitionResult[i]);
	}
	printf("\n");

	printf("<Bottom Die Data>: contains %d Standard Cells\n", ArrayInfo.BottomCellNumber);
	for(int i = 0; i < ArrayInfo.BottomCellNumber; i++){
		printf("Cell name: %s\n", InstanceArray[ArrayInfo.BottomCellArray[i].cellID].instName);
		printf("Been Partition In %d\n", ArrayInfo.BottomCellArray[i].WhichDie);
	}

	printf("<Top Die Data>: contains %d Standard Cells\n", ArrayInfo.TopCellNumber);
	for(int i = 0; i < ArrayInfo.TopCellNumber; i++){
		printf("Cell name: %s\n", InstanceArray[ArrayInfo.TopCellArray[i].cellID].instName);
		printf("Been Partition In %d\n", ArrayInfo.TopCellArray[i].WhichDie);
	}
}

void GetCellOfNet(vector <RawNet> rawnet, vector <Net> &NetArray, int NumNets){
	Net tempNet;
	for(int i=0; i<NumNets; i++){
		vector <int> temp_cells;
		for(int j=0; j<(rawnet[i].numPins); j++){
			char buffer[TEMP_BUFFER_SIZE];
			memset(buffer, '\0', sizeof(buffer));
			strncpy(buffer,  &(rawnet[i].Connection[j].instName[1]), strlen(rawnet[i].Connection[j].instName)-1);
			int temp_cellID = atoi(buffer) -1;
			temp_cells.emplace_back(temp_cellID);
		}
		tempNet.cells = temp_cells;
		tempNet.CellNumber = rawnet[i].numPins;
		tempNet.NetID = i;
		NetArray.emplace_back(tempNet);
	}
}

void PrintNetArray(vector <Net> NetArray, int NumNets){
	for(int i=0; i<NumNets; i++){
		printf("Cell in NetID=%d :\n", NetArray[i].NetID);
		printf("contains %d cells.\n", NetArray[i].CellNumber);
		for(int j=0; j<NetArray[i].CellNumber; j++){
			printf("%d ", NetArray[i].cells[j]);
		}
		printf("\n");
	}
}

void GetNetOfCell(vector <Net> NetArray, TopBottomCellArray *ArrayInfo, vector <int> PartitionResult){
	vector <int> PartitionIndexResult;
	int bottomCount = 0;
	int topCount = 0;
	for(int i = 0; i < (int)PartitionResult.size(); i++){
		if(PartitionResult[i] == 0){
			PartitionIndexResult[i] = bottomCount;
			bottomCount++;
		}
		else{
			PartitionIndexResult[i] = topCount;
			topCount++;
		}
	}



	for(int i = 0 ; i < (int)NetArray.size(); i++){
		for(int j=0; j<NetArray[i].CellNumber; j++){
			int currentCellIndex = NetArray[i].cells[j];
			if(PartitionResult[currentCellIndex]==0){
				ArrayInfo.BottomCellArray[PartitionIndexResult[currentCellIndex]]
			}
			ArrayInfo
		}
	}
}

