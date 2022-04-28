#include "readfile.h"

#ifndef _PARTITION_H_
#define _PARTITION_H_

void OutputPartitionFormat(int NumNets, int NumInstances, vector <RawNet> NetArray);
void PartitionInstance();
void ReadPartitionResult(vector <Instance> &InstanceArray, int NumInstances);
void printPartitionResult(vector <Instance> InstanceArray, int NumInstances);
#endif