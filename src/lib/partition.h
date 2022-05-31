#include "readfile.h"
#include <vector>

using namespace std;
using std::vector;

#ifndef _PARTITION_H_
#define _PARTITION_H_

typedef struct netStruct{
	int CellNumber;      			//cell number included
	vector <int> cells;  			//list of cell included in this nell
	int NetID;
	bool hasHybridTerminal;
	int x_min;
	int x_max;
	int y_min;
	int y_max;
	int HBlocationX;
	int HBLocationY;            			
} Net;

typedef struct cellStruct{
	int netsNumber; 				//net number connect to this cell
	vector <int> nets; 				//a list of nets connected to this cell
	int WhichDie;					//(0, 1) = (bottom die, top die)
	int cellID;						//the cellid number must meet the index of instancearray
    int libCellSizeX;				
    int libCellSizeY;
    int rowID;
    int left_edge;
    int right_edge;
} Cell;

typedef struct _TopBottomCellArray{
	int BottomCellNumber;
	int TopCellNumber;
	vector <int> PartitionIndexResult;
	vector <Cell> BottomCellArray;
	vector <Cell> TopCellArray;
} TopBottomCellArray;

void OutputPartitionFormat(int NumNets, int NumInstances, vector <RawNet> rawnet, vector <Tech_menu> TechMenu, vector <Instance> InstanceArray, Die top_die, Die bottom_die);
void PartitionInstance(int UBfactor);
void ReadCutSize(int *NumTerminal);
void ReadPartitionResult(TopBottomCellArray *ArrayInfo, int NumInstances, vector <int> &PartitionResult);
void UpdateInstanceArray(vector <Instance> &InstanceArray, vector <int> PartitionResult, Die top_die, Die bottom_die);
void printPartitionResult(TopBottomCellArray ArrayInfo, vector <Instance> InstanceArray, vector <int> PartitionResult);
void GetCellOfNet(vector <RawNet> rawnet, vector <Net> &NetArray, int NumNets, vector <int> PartitionResult, int *NumTerminal);
void PrintNetArray(vector <Net> NetArray, int NumNets);
void GetNetOfCell(vector <Net> NetArray, TopBottomCellArray *ArrayInfo, vector <int> PartitionResult);
void getSizeOfCellArray(TopBottomCellArray *ArrayInfo, vector <Tech_menu> TechMenu, Die top_die, Die bottom_die, vector <Instance> InstanceArray);
void printTopBottomCellArray(TopBottomCellArray *ArrayInfo, vector <int> PartitionResult);
#endif