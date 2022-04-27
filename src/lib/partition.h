#include "readfile.h"

#ifndef _PARTITION_H_
#define _PARTITION_H_

void OutputPartitionFormat(int NumNets, int NumInstances, Net *NetArray);
void PartitionInstance();
void ReadPartitionResult(Instance *InstanceArray, int NumInstances);
void printPartitionResult(Instance *InstanceArray, int NumInstances);
#endif