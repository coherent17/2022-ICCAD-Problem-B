#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "partition.h"
#include "readfile.h"

#define TEMP_BUFFER_SIZE 25
#define WORD_BUFFER_SIZE 1000

void OutputPartitionFormat(int NumNets, int NumInstances, vector <RawNet> rawnet, vector <Tech_menu> TechMenu, vector <Instance> InstanceArray, Die top_die, Die bottom_die){
	FILE *shmetisInput = fopen("Netlist.hgr", "w");
	fprintf(shmetisInput, "%d %d\n", NumNets, NumInstances);

	for(int i = 0; i < NumNets; i++){
		for(int j = 0; j < rawnet[i].numPins; j++){
			char buffer[TEMP_BUFFER_SIZE];
			memset(buffer, '\0', sizeof(buffer));
			strncpy(buffer,  &(rawnet[i].Connection[j].instName[1]), strlen(rawnet[i].Connection[j].instName)-1);
			fprintf(shmetisInput, "%s ", buffer);
		}
		fprintf(shmetisInput, "\n");
	}

	//start to ouput the weighted vertex
	// for(int i = 0; i < NumInstances; i++){
	// 	int weight = 0;
	// 	char buffer[LIBCELL_NAME_SIZE];
	// 	memset(buffer, '\0', sizeof(buffer));
	// 	strncpy(buffer, InstanceArray[i].libCellName+2, strlen(InstanceArray[i].libCellName)-2);


	// 	for(int j = 0; j < (int)TechMenu.size(); j++){
	// 		if(strcmp(TechMenu[j].tech, bottom_die.tech)==0){
	// 			weight += (int)TechMenu[j].libcell[atoi(buffer)-1].libCellSizeX / bottom_die.repeatCount;
	// 		}
	// 		else{
	// 			weight += (int)TechMenu[j].libcell[atoi(buffer)-1].libCellSizeX / top_die.repeatCount;
	// 		}
	// 	}
	// 	fprintf(shmetisInput, "%d\n", weight);
	// }
	fclose(shmetisInput);
}

//using shmetis to do 2-way partition and redirect the output stream to log.txt
void PartitionInstance(int UBfactor){
	system("chmod +x src/hmetis/shmetis");
	char command[100];
	sprintf(command,"./src/hmetis/shmetis Netlist.hgr 2 %d > log.txt", UBfactor);
	system(command);

	//what if the partition result still can't have a valid placement?
	//change unblanced factor?
	//random the vertex weight?
}

//read cut size 
void ReadCutSize(int *NumTerminal){
	FILE *input=fopen("log.txt","r");
    char current[WORD_BUFFER_SIZE];
	char last[WORD_BUFFER_SIZE];
	while(fscanf(input,"%s",current)!=EOF){
		if(strncmp(current,"Hyperedge",9)==0){
			strcpy(last,current);
		}else if((strncmp(current,"Cut:",4)==0) && strncmp(last,"Hyperedge",8)==0){
			fscanf(input,"%d",&(*NumTerminal));
			printf("Numterminal = %d \n", *NumTerminal);
			fclose(input);
			break;
		}
	}
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
	system("rm -rf Netlist.hgr Netlist.hgr.part.2");
}

//After partition, update the instanceArray.tech 
void UpdateInstanceArray(vector <Instance> &InstanceArray, vector <int> PartitionResult, Die top_die, Die bottom_die){
	for(int i = 0; i < (int)InstanceArray.size(); i++){
		if(PartitionResult[i] == 0){
			strcpy(InstanceArray[i].tech, bottom_die.tech);
		}
		else if(PartitionResult[i] == 1){
			strcpy(InstanceArray[i].tech, top_die.tech);
		}
		else {
			assert(PartitionResult[i] != 0 || PartitionResult[i] != 1);
		}
	}
}

void printPartitionResult(TopBottomCellArray ArrayInfo, vector <Instance> InstanceArray, vector <int> PartitionResult){
	for(int i = 0; i < (int)PartitionResult.size(); i++){
		printf("%d ", PartitionResult[i]);
	}
	printf("\n");

	printf("<Bottom Die Data>: contains %d Cells\n", ArrayInfo.BottomCellNumber);
	for(int i = 0; i < ArrayInfo.BottomCellNumber; i++){
		printf("Cell name: %s\n", InstanceArray[ArrayInfo.BottomCellArray[i].cellID].instName);
		printf("Cell tech: %s\n", InstanceArray[ArrayInfo.BottomCellArray[i].cellID].tech);
		printf("Been Partition In %d\n", ArrayInfo.BottomCellArray[i].WhichDie);
	}

	printf("<Top Die Data>: contains %d Cells\n", ArrayInfo.TopCellNumber);
	for(int i = 0; i < ArrayInfo.TopCellNumber; i++){
		printf("Cell name: %s\n", InstanceArray[ArrayInfo.TopCellArray[i].cellID].instName);
		printf("Cell tech: %s\n", InstanceArray[ArrayInfo.TopCellArray[i].cellID].tech);
		printf("Been Partition In %d\n", ArrayInfo.TopCellArray[i].WhichDie);
	}
}

void GetCellOfNet(vector <RawNet> rawnet, vector <Net> &NetArray, int NumNets, vector <int> PartitionResult){
	Net tempNet;
	int terminal_count = 0;
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

		//calculate whether there exist hybrid bond terminal
		tempNet.hasHybridTerminal = false;
		int first_cell_partition = PartitionResult[temp_cells[0]];
		for(int j = 1; j < (int)temp_cells.size(); j++){
			if(PartitionResult[temp_cells[j]] != first_cell_partition){
				terminal_count++;
				tempNet.hasHybridTerminal = true;
				break;
			}
		}
		NetArray.emplace_back(tempNet);
	}
}

void PrintNetArray(vector <Net> NetArray, int NumNets){
	for(int i=0; i<NumNets; i++){
		printf("Cell in NetID=%d : ", NetArray[i].NetID);
		printf("contains %d cells.\n", NetArray[i].CellNumber);
		for(int j=0; j<NetArray[i].CellNumber; j++){
			printf("%d ", NetArray[i].cells[j]);
		}
		printf("\n");

		if(NetArray[i].hasHybridTerminal) printf("There exists hasHybridTerminal in this net.\n");
	}
}

void GetNetOfCell(vector <Net> NetArray, TopBottomCellArray *ArrayInfo, vector <int> PartitionResult){
	vector <int> PartitionIndexResult(PartitionResult.size());
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

	ArrayInfo->PartitionIndexResult = PartitionIndexResult;

	//temp_netsNumber store the number of the nets that connect to the cell
	//ex: # of nets connect to C1 = temp_netsNumber[0]
	vector <int> temp_netsNumber(PartitionResult.size(),0);
	vector <vector <int>> temp_nets(PartitionResult.size());

	for(int i = 0; i < (int)NetArray.size(); i++){
		for(int j = 0; j < NetArray[i].CellNumber; j++){
			temp_netsNumber[NetArray[i].cells[j]]++;
			temp_nets[NetArray[i].cells[j]].emplace_back(i);
		}
	}


	for(int i=0; i<(int)PartitionResult.size(); i++){
		if( PartitionResult[i] == 0){
			ArrayInfo->BottomCellArray[PartitionIndexResult[i]].netsNumber = temp_netsNumber[i];
			ArrayInfo->BottomCellArray[PartitionIndexResult[i]].nets = temp_nets[i];
		}
		else if( PartitionResult[i] == 1){
			ArrayInfo->TopCellArray[PartitionIndexResult[i]].netsNumber = temp_netsNumber[i];
			ArrayInfo->TopCellArray[PartitionIndexResult[i]].nets = temp_nets[i];
		}
		assert(PartitionResult[i]!=0 || PartitionResult[i]!=1 );
	}
}



void getSizeOfCellArray(TopBottomCellArray *ArrayInfo, vector <Tech_menu> TechMenu, Die top_die, Die bottom_die, vector <Instance> InstanceArray){	
	
	//start from bottomDie
	char bottomTech[TECH_NAME_SIZE];
	memset(bottomTech, '\0', TECH_NAME_SIZE);
	strncpy(bottomTech, bottom_die.tech, strlen(bottom_die.tech));
	for(int i = 0; i < ArrayInfo->BottomCellNumber; i++){
		//get current libcellname from instance array
		char current_libCellName[LIBCELL_NAME_SIZE];
		memset(current_libCellName,'\0', LIBCELL_NAME_SIZE);
		strncpy(current_libCellName, InstanceArray[ArrayInfo->BottomCellArray[i].cellID].libCellName+2, strlen(InstanceArray[ArrayInfo->BottomCellArray[i].cellID].libCellName)-2);

		for(int j = 0; j < (int)TechMenu.size(); j++){
			if(strncmp(TechMenu[j].tech, bottomTech, strlen(bottomTech)) == 0){
				ArrayInfo->BottomCellArray[i].libCellSizeX = TechMenu[j].libcell[atoi(current_libCellName)-1].libCellSizeX;
				ArrayInfo->BottomCellArray[i].libCellSizeY = TechMenu[j].libcell[atoi(current_libCellName)-1].libCellSizeY;
			}
		}
	}

	//start from topDie
	char topTech[TECH_NAME_SIZE];
	memset(topTech, '\0', TECH_NAME_SIZE);
	strncpy(topTech, top_die.tech, strlen(top_die.tech));
	for(int i = 0; i < ArrayInfo->TopCellNumber; i++){
		//get current libcellname from instance array
		char current_libCellName[LIBCELL_NAME_SIZE];
		memset(current_libCellName,'\0', LIBCELL_NAME_SIZE);
		strncpy(current_libCellName, InstanceArray[ArrayInfo->TopCellArray[i].cellID].libCellName+2, strlen(InstanceArray[ArrayInfo->TopCellArray[i].cellID].libCellName)-2);

		for(int j = 0; j < (int)TechMenu.size(); j++){
			if(strncmp(TechMenu[j].tech, topTech, strlen(topTech)) == 0){
				ArrayInfo->TopCellArray[i].libCellSizeX = TechMenu[j].libcell[atoi(current_libCellName)-1].libCellSizeX;
				ArrayInfo->TopCellArray[i].libCellSizeY = TechMenu[j].libcell[atoi(current_libCellName)-1].libCellSizeY;
			}
		}
	}
}


void printTopBottomCellArray(TopBottomCellArray *ArrayInfo, vector <int> PartitionResult){
	printf("PartiionResult: \n");
	for(int i=0; i< (int)PartitionResult.size(); i++){
		printf("%d  ",PartitionResult[i]);
	}
	printf("\n");
	printf("PartitionIndexResult: \n");
	for(int i=0; i< (int)ArrayInfo->PartitionIndexResult.size(); i++){
		printf("%d  ",ArrayInfo->PartitionIndexResult[i]);
	}
	printf("\n");

	printf("BottomCell Number: %d\n", ArrayInfo->BottomCellNumber);
	for(int i = 0; i < ArrayInfo->BottomCellNumber; i++){
		printf("\tCell ID: %d\n", ArrayInfo->BottomCellArray[i].cellID);
		printf("\t\tBelongs to %d Die\n", ArrayInfo->BottomCellArray[i].WhichDie);
		printf("\t\tThere are %d nets connect to this cell ", ArrayInfo->BottomCellArray[i].netsNumber);
		for(int j = 0; j < ArrayInfo->BottomCellArray[i].netsNumber; j++){
			printf("%d ", ArrayInfo->BottomCellArray[i].nets[j]);
		}
		printf("\t\t(libCellSizeX, libCellSizeY) = (%d, %d)\n", ArrayInfo->BottomCellArray[i].libCellSizeX, ArrayInfo->BottomCellArray[i].libCellSizeY);
		printf("\n");
	}

	printf("TopCell Number: %d\n", ArrayInfo->TopCellNumber);
	for(int i = 0; i < ArrayInfo->TopCellNumber; i++){
		printf("\tCell ID: %d\n", ArrayInfo->TopCellArray[i].cellID);
		printf("\t\tBelongs to %d Die\n", ArrayInfo->TopCellArray[i].WhichDie);
		printf("\t\tThere are %d nets connect to this cell ", ArrayInfo->TopCellArray[i].netsNumber);
		for(int j = 0; j < ArrayInfo->TopCellArray[i].netsNumber; j++){
			printf("%d ", ArrayInfo->TopCellArray[i].nets[j]);
		}
		printf("\t\t(libCellSizeX, libCellSizeY) = (%d, %d)\n", ArrayInfo->TopCellArray[i].libCellSizeX, ArrayInfo->TopCellArray[i].libCellSizeY);
		printf("\n");
	}
}