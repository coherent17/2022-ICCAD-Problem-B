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
	vector <RawNet> NetArray;

	//read file part
	readTechnologyInfo(input, &NumTechnologies, TechMenu);
	printTechnologyInfo(NumTechnologies, TechMenu);
	readDieInfo(input, &top_die, &bottom_die);
	printDieInfo(top_die, bottom_die);
	readHybridTerminalInfo(input, &terminal);
	printHybridTerminalInfo(terminal);
	readInstanceInfo(input, &NumInstances, InstanceArray);
	printInstanceInfo(NumInstances, InstanceArray);
	readNetInfo(input, &NumNets, NetArray);
	printNetInfo(NumNets, NetArray);


	//partition part
	OutputPartitionFormat(NumNets, NumInstances, NetArray);
	PartitionInstance();
	ReadPartitionResult(InstanceArray, NumInstances);
	printPartitionResult(InstanceArray, NumInstances);

	return 0;
}