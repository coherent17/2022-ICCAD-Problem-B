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

	Die top_die, bottom_die;
	Hybrid_terminal terminal;
	int NumTechnologies;
	vector <Tech_menu> TechMenu;
	int NumInstances;
	vector <Instance> InstanceArray;
	int NumNets;
	vector <RawNet> rawnet;

	vector <int> PartitionResult;
	vector <Net> NetArray;
	TopBottomCellArray ArrayInfo;

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
	OutputPartitionFormat(NumNets, NumInstances, rawnet);
	PartitionInstance();
	ReadPartitionResult(&ArrayInfo, NumInstances, PartitionResult);
	printPartitionResult(ArrayInfo, InstanceArray, PartitionResult);


	//create netarray and cellarray
	GetCellOfNet(rawnet, NetArray, NumNets);
	PrintNetArray(NetArray, NumNets);
	GetNetOfCell(NetArray, &ArrayInfo, PartitionResult);
	printTopBottomCellArray(&ArrayInfo);

	return 0;
}