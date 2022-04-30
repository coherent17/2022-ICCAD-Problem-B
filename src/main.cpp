#include <stdio.h>
#include <assert.h>
#include <vector>
#include <stdlib.h>
#include "../src/lib/readfile.h"
#include "../src/lib/partition.h"
#include "../src/lib/placement.h"

using namespace std;
using std::vector;

int main(int argc, char *argv[]){
	char *inputName = *(argv + 1);
	FILE *input = fopen(inputName, "r");
	assert(input);

	Die top_die, bottom_die;												//store the die information
	Hybrid_terminal terminal;												//store the size of the hybrid bond terminal connect between two dies
	int NumTerminal;														//cut size calculated by shmetis
	int NumTechnologies;													//TA and TB
	vector <Tech_menu> TechMenu;											//The detail of the library of the standardcell by different technology
	int NumInstances;														//How many instances need to be placeed in the two dies
	vector <Instance> InstanceArray;										//The standard cell with its library
	int NumNets;															//How many nets connect betweem Instances
	vector <RawNet> rawnet;													//The rawnet data store in input

	vector <int> PartitionResult;											//Using hmetis to do 2-way partition to divide the Instances into to die
	vector <Net> NetArray;													//NetArray store how many cell connects to this net and a list of cell which connects to this net
	TopBottomCellArray ArrayInfo;											//Store the partition result and there is two vector inside this structure (see partition.h) store the cellarray of the two dies


	//read file part
	readTechnologyInfo(input, &NumTechnologies, TechMenu);			
	printTechnologyInfo(NumTechnologies, TechMenu);
	readDieInfo(input, &top_die, &bottom_die);
	printDieInfo(top_die, bottom_die);
	readHybridTerminalInfo(input, &terminal);
	printHybridTerminalInfo(terminal);
	readInstanceInfo(input, &NumInstances, InstanceArray);
	printInstanceInfo(NumInstances, InstanceArray);
	readNetInfo(input, &NumNets, rawnet);
	printNetInfo(NumNets, rawnet);


	//partition part
	OutputPartitionFormat(NumNets, NumInstances, rawnet);					//convert the rawnet into the file that can feed to shmetis to do partition
	PartitionInstance();													//using shmetis to perform two way partition
	ReadCutSize(&NumTerminal);									            //read cut size
	ReadPartitionResult(&ArrayInfo, NumInstances, PartitionResult);			//store the partition result into cellarray in a
	UpdateInstanceArray(InstanceArray, PartitionResult, top_die, bottom_die);
	printPartitionResult(ArrayInfo, InstanceArray, PartitionResult);


	//create netarray and cellarray
	GetCellOfNet(rawnet, NetArray, NumNets);
	PrintNetArray(NetArray, NumNets);
	GetNetOfCell(NetArray, &ArrayInfo, PartitionResult);
	getSizeOfCellArray(&ArrayInfo, TechMenu, top_die, bottom_die, InstanceArray);
	printTopBottomCellArray(&ArrayInfo, PartitionResult);


	//initial placement
	bool BottomPartitionAgain;
	bool TopPartitionAgain;
	InitializePlacement(&bottom_die, ArrayInfo, 0, &BottomPartitionAgain);
	printPlacementState(bottom_die, BottomPartitionAgain);
	InitializePlacement(&top_die, ArrayInfo, 1, &TopPartitionAgain);
	printPlacementState(top_die, TopPartitionAgain);


	//repartition if the current can't have a legal placement
	int repartitionCount = 0;
	while(BottomPartitionAgain == true || TopPartitionAgain == true){
		repartitionCount++;

		//clear the partition result
		PartitionResult.clear();
		for(int i = 0; i < ArrayInfo.BottomCellNumber; i++){
			ArrayInfo.BottomCellArray[i].nets.clear();
		}
		for(int i = 0 ; i < ArrayInfo.TopCellNumber; i++){
			ArrayInfo.TopCellArray[i].nets.clear();
		}
		ArrayInfo.PartitionIndexResult.clear();
		ArrayInfo.BottomCellArray.clear();
		ArrayInfo.TopCellArray.clear();


		//partition part
		OutputPartitionFormat(NumNets, NumInstances, rawnet);					//convert the rawnet into the file that can feed to shmetis to do partition
		PartitionInstance();													//using shmetis to perform two way partition
		ReadCutSize(&NumTerminal);									            //read cut size
		ReadPartitionResult(&ArrayInfo, NumInstances, PartitionResult);			//store the partition result into cellarray in a
		UpdateInstanceArray(InstanceArray, PartitionResult, top_die, bottom_die);
		printPartitionResult(ArrayInfo, InstanceArray, PartitionResult);


		//create netarray and cellarray
		PrintNetArray(NetArray, NumNets);
		GetNetOfCell(NetArray, &ArrayInfo, PartitionResult);
		getSizeOfCellArray(&ArrayInfo, TechMenu, top_die, bottom_die, InstanceArray);
		printTopBottomCellArray(&ArrayInfo, PartitionResult);

		InitializePlacement(&bottom_die, ArrayInfo, 0, &BottomPartitionAgain);
		printPlacementState(bottom_die, BottomPartitionAgain);
		InitializePlacement(&top_die, ArrayInfo, 1, &TopPartitionAgain);
		printPlacementState(top_die, TopPartitionAgain);
	}
	printf("repartition %d times\n", repartitionCount);
	return 0;
}