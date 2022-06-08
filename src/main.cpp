#include <stdio.h>
#include <assert.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "../src/lib/readfile.h"
#include "../src/lib/partition.h"
#include "../src/lib/placement.h"
#include "../src/lib/random.h"
#include "../src/lib/simulate_annealing.h"

double CUT_OFF_TIME;
double start_time;

using namespace std;
using std::vector;

int main(int argc, char *argv[]){
	CUT_OFF_TIME = 1800;
	start_time = omp_get_wtime();
	srand(time(NULL));
	char *inputName = *(argv + 1);
	char *outputName = *(argv + 2);
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
	//printTechnologyInfo(NumTechnologies, TechMenu);
	readDieInfo(input, &top_die, &bottom_die);
	//printDieInfo(top_die, bottom_die);
	readHybridTerminalInfo(input, &terminal);
	//printHybridTerminalInfo(terminal);
	readInstanceInfo(input, &NumInstances, InstanceArray);
	//printInstanceInfo(NumInstances, InstanceArray);
	readNetInfo(input, &NumNets, rawnet);
	//printNetInfo(NumNets, rawnet);


	//partition part
	OutputPartitionFormat(NumNets, NumInstances, rawnet, TechMenu, InstanceArray, top_die, bottom_die);					//convert the rawnet into the file that can feed to shmetis to do partition
	PartitionInstance(5);													//using shmetis to perform two way partition
	ReadCutSize(&NumTerminal);									            //read cut size
	ReadPartitionResult(&ArrayInfo, NumInstances, PartitionResult);			//store the partition result into cellarray in a
	UpdateInstanceArray(InstanceArray, PartitionResult, top_die, bottom_die);
	//printPartitionResult(ArrayInfo, InstanceArray, PartitionResult);


	//create netarray and cellarray
	GetCellOfNet(rawnet, NetArray, NumNets, PartitionResult, &NumTerminal);
	//PrintNetArray(NetArray, NumNets);
	GetNetOfCell(NetArray, &ArrayInfo, PartitionResult);
	getSizeOfCellArray(&ArrayInfo, TechMenu, top_die, bottom_die, InstanceArray);
	//printTopBottomCellArray(&ArrayInfo, PartitionResult);


	//initial placement
	bool BottomPartitionAgain;
	bool TopPartitionAgain;
	InitializePlacement(&bottom_die, &ArrayInfo, 0, &BottomPartitionAgain);
	//printPlacementState(bottom_die, BottomPartitionAgain);
	if(BottomPartitionAgain == false){
		InitializePlacement(&top_die, &ArrayInfo, 1, &TopPartitionAgain);
		//printPlacementState(top_die, TopPartitionAgain);
	}

	//repartition if the current can't have a legal placement
	int repartitionCount = 0;
	int UBfactor = 5;
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
		NetArray.clear();


		//partition part
		OutputPartitionFormat(NumNets, NumInstances, rawnet, TechMenu, InstanceArray, top_die, bottom_die);	
		PartitionInstance(UBfactor);
		ReadCutSize(&NumTerminal);
		ReadPartitionResult(&ArrayInfo, NumInstances, PartitionResult);
		UpdateInstanceArray(InstanceArray, PartitionResult, top_die, bottom_die);
		//printPartitionResult(ArrayInfo, InstanceArray, PartitionResult);


		//create netarray and cellarray
		GetCellOfNet(rawnet, NetArray, NumNets, PartitionResult, &NumTerminal);
		//PrintNetArray(NetArray, NumNets);
		GetNetOfCell(NetArray, &ArrayInfo, PartitionResult);
		getSizeOfCellArray(&ArrayInfo, TechMenu, top_die, bottom_die, InstanceArray);
		//printTopBottomCellArray(&ArrayInfo, PartitionResult);

		InitializePlacement(&bottom_die, &ArrayInfo, 0, &BottomPartitionAgain);
		//printPlacementState(bottom_die, BottomPartitionAgain);
		if(BottomPartitionAgain == false){
			InitializePlacement(&top_die, &ArrayInfo, 1, &TopPartitionAgain);
			//printPlacementState(top_die, TopPartitionAgain);
		}
		if(repartitionCount % 200 == 0) UBfactor += 3;
		UBfactor = UBfactor % 35 + 1; 
	}
	printf("repartition %d times\n", repartitionCount);
	//OutputCellLocateState(ArrayInfo, top_die, bottom_die, rawnet, TechMenu, PartitionResult, InstanceArray);



	//Construct Simulate Annealing Content Struct:
	SA_CONTENT OLD_SA_contentPtr;
	OLD_SA_contentPtr.top_die = top_die;
	OLD_SA_contentPtr.bottom_die = bottom_die;
	OLD_SA_contentPtr.TechMenu = TechMenu;
	OLD_SA_contentPtr.rawnet = rawnet;
	OLD_SA_contentPtr.PartitionResult = PartitionResult;
	OLD_SA_contentPtr.InstanceArray = InstanceArray;
	OLD_SA_contentPtr.ArrayInfo = ArrayInfo;


	SA_CONTENT SA_contentPtr;
	SA_contentPtr.top_die = top_die;
	SA_contentPtr.bottom_die = bottom_die;
	SA_contentPtr.TechMenu = TechMenu;
	SA_contentPtr.rawnet = rawnet;
	SA_contentPtr.PartitionResult = PartitionResult;
	SA_contentPtr.InstanceArray = InstanceArray;
	SA_contentPtr.ArrayInfo = ArrayInfo;


	//simulate annealing parameter
	unsigned long long int INITIAL_COST;
	double ANNEALING_TEMPERATURE;
	double TERMINATE_TEMPERATURE;
	int INNER_LOOP_TIMES;
	double ALPHA;

	//using high temperature to seperate the instance
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	INITIAL_COST = Cost(SA_contentPtr);
	ANNEALING_TEMPERATURE = double(200 * INITIAL_COST);
	TERMINATE_TEMPERATURE = ANNEALING_TEMPERATURE * 0.9;
	INNER_LOOP_TIMES = int(0.25 * NumInstances);
	ALPHA = 0.95;

	SA_contentPtr = SimulateAnnealing(SA_contentPtr, ANNEALING_TEMPERATURE, TERMINATE_TEMPERATURE, INNER_LOOP_TIMES, ALPHA);
	// top_die = SA_contentPtr.top_die;
	// bottom_die = SA_contentPtr.bottom_die;
	// ArrayInfo = SA_contentPtr.ArrayInfo;
	//OutputCellLocateState(ArrayInfo, top_die, bottom_die, rawnet, TechMenu, PartitionResult, InstanceArray);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	INITIAL_COST = Cost(SA_contentPtr);
	ANNEALING_TEMPERATURE = double(INITIAL_COST);
	TERMINATE_TEMPERATURE = 0.000005 * INITIAL_COST / NumNets;
	INNER_LOOP_TIMES = int(0.5 * NumInstances);
	ALPHA = 0.95;

	SA_contentPtr = SimulateAnnealing(SA_contentPtr, ANNEALING_TEMPERATURE, TERMINATE_TEMPERATURE, INNER_LOOP_TIMES, ALPHA);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//compare the placement after simulate annealing with before simulate annealing
	printf("%lld %lld\n", Cost(SA_contentPtr), Cost(OLD_SA_contentPtr));
	if(Cost(SA_contentPtr) <  Cost(OLD_SA_contentPtr)){
		top_die = SA_contentPtr.top_die;
		bottom_die = SA_contentPtr.bottom_die;
		ArrayInfo = SA_contentPtr.ArrayInfo;
		StoreBBOX(SA_contentPtr, NetArray);
	}
	else{
		top_die = OLD_SA_contentPtr.top_die;
		bottom_die = OLD_SA_contentPtr.bottom_die;
		ArrayInfo = OLD_SA_contentPtr.ArrayInfo;
		StoreBBOX(OLD_SA_contentPtr, NetArray);	
	}

	OutputCellLocateState(ArrayInfo, top_die, bottom_die, rawnet, TechMenu, PartitionResult, InstanceArray);

	//place the hybridBonding Termimal
	HybridPlacement(&terminal, top_die, NetArray);
	outputAnswer(outputName, ArrayInfo, top_die, bottom_die, NumTerminal, NetArray);
	return 0;
}