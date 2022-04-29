#include <stdio.h>
#include <assert.h>
#include <vector>
#include "../src/lib/readfile.h"
#include "../src/lib/partition.h"

using namespace std;
using std::vector;

int main(int argc, char *argv[]){
	char *inputName = *(argv + 1);
	FILE *input = fopen(inputName, "r");
	assert(input);

	Die top_die, bottom_die;												//store the die information
	Hybrid_terminal terminal;												//store the size of the hybrid bond terminal connect between two dies
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
	ReadPartitionResult(&ArrayInfo, NumInstances, PartitionResult);			//store the partition result into cellarray in a
	printPartitionResult(ArrayInfo, InstanceArray, PartitionResult);


	//create netarray and cellarray
	GetCellOfNet(rawnet, NetArray, NumNets);
	PrintNetArray(NetArray, NumNets);
	GetNetOfCell(NetArray, &ArrayInfo, PartitionResult);
	printTopBottomCellArray(&ArrayInfo);

	return 0;
}