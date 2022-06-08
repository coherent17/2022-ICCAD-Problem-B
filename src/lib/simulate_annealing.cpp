#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <omp.h>
#include "placement.h"
#include "readfile.h"
#include "partition.h"
#include "random.h"
#include "simulate_annealing.h"

bool SA_isValidPlacement(vector <vector <int>> PlacementState, int row, int left_edge, int right_edge){
    
    for(int i=left_edge; i<=right_edge; i++){
        if(PlacementState[row][i]!=EMPTY_STATE){
            return false;
        }
    }
    return true;
}

unsigned long long int Cost(SA_CONTENT SA_contentPtr){

	//unpack the SA_contentPtr
	Die top_die = SA_contentPtr.top_die;
	Die bottom_die = SA_contentPtr.bottom_die;
	vector <Tech_menu> TechMenu = SA_contentPtr.TechMenu;
	vector <RawNet> rawnet = SA_contentPtr.rawnet;
	vector <int> PartitionResult = SA_contentPtr.PartitionResult;
	vector <Instance> InstanceArray = SA_contentPtr.InstanceArray;
	TopBottomCellArray ArrayInfo = SA_contentPtr.ArrayInfo;


    vector <bbox> bboxes;

    //output the pin location
    for(int i = 0; i < (int)rawnet.size(); i++){

        //the record for the bounding box
        int x_min = INT_MAX;
        int x_max = -1;
        int y_min = INT_MAX;
        int y_max = -1;

        //find the pin location that rawnet[i] connect to
        for(int j=0; j <rawnet[i].numPins; j++){
            char buffer[INSTANCE_NAME_SIZE];
            memset(buffer,'\0',INSTANCE_NAME_SIZE);
            char PinName[PIN_NAME_SIZE];
            memset(PinName,'\0',PIN_NAME_SIZE);
            strncpy(PinName, &rawnet[i].Connection[j].libPinName[1], strlen(rawnet[i].Connection[j].libPinName)-1);
            strncpy(buffer, &rawnet[i].Connection[j].instName[1], strlen(rawnet[i].Connection[j].instName)-1);
            int current_instance = atoi(buffer);

            //check the instance belongs to which die
            int current_instance_partition = PartitionResult[current_instance-1];
            int index = ArrayInfo.PartitionIndexResult[current_instance-1];
            int reference_left_edge = 0;
            int reference_row = 0;
            char current_tech[TECH_NAME_SIZE];
            memset(current_tech,'\0',TECH_NAME_SIZE);
            char libCellName[LIBCELL_NAME_SIZE];
            memset(libCellName,'\0',LIBCELL_NAME_SIZE);

            if(current_instance_partition == 0){
                reference_left_edge = ArrayInfo.BottomCellArray[index].left_edge;
                reference_row = ArrayInfo.BottomCellArray[index].rowID * bottom_die.rowHeight;
                strcpy(current_tech, bottom_die.tech);
                strncpy(libCellName, &InstanceArray[current_instance-1].libCellName[2], strlen(InstanceArray[current_instance-1].libCellName)-2);
            }

            else if(current_instance_partition == 1){
                reference_left_edge = ArrayInfo.TopCellArray[index].left_edge;
                reference_row = ArrayInfo.TopCellArray[index].rowID * top_die.rowHeight;
                strcpy(current_tech, top_die.tech);
                strncpy(libCellName, &InstanceArray[current_instance-1].libCellName[2], strlen(InstanceArray[current_instance-1].libCellName)-2);
            }

            for(int k=0; k < (int)TechMenu.size(); k++){
                if(strcmp(current_tech, TechMenu[k].tech) == 0){
                	x_min = (reference_left_edge + TechMenu[k].libcell[atoi(libCellName)-1].pinarray[atoi(PinName)-1].pinLocationX) < x_min ? (reference_left_edge + TechMenu[k].libcell[atoi(libCellName)-1].pinarray[atoi(PinName)-1].pinLocationX) : x_min ;
                	x_max = (reference_left_edge + TechMenu[k].libcell[atoi(libCellName)-1].pinarray[atoi(PinName)-1].pinLocationX) > x_max ? (reference_left_edge + TechMenu[k].libcell[atoi(libCellName)-1].pinarray[atoi(PinName)-1].pinLocationX) : x_max ;
					y_min = (reference_row + TechMenu[k].libcell[atoi(libCellName)-1].pinarray[atoi(PinName)-1].pinLocationY) < y_min ? (reference_row + TechMenu[k].libcell[atoi(libCellName)-1].pinarray[atoi(PinName)-1].pinLocationY) : y_min;
					y_max = (reference_row + TechMenu[k].libcell[atoi(libCellName)-1].pinarray[atoi(PinName)-1].pinLocationY) > y_max ? (reference_row + TechMenu[k].libcell[atoi(libCellName)-1].pinarray[atoi(PinName)-1].pinLocationY) : y_max;
                }
            }            

        }
        bbox temp_bbox;
        temp_bbox.x_min = x_min;
        temp_bbox.x_max = x_max;
        temp_bbox.y_min = y_min;
        temp_bbox.y_max = y_max;
        bboxes.emplace_back(temp_bbox); 
    }

    unsigned long long int result = 0;
    //calculate the total similar_HPWL of the bbox
    for(int i = 0; i < (int)bboxes.size(); i++){
    	result += (bboxes[i].x_max - bboxes[i].x_min) + (bboxes[i].y_max - bboxes[i].y_min);
    }
    return result;
}

//return the placement state of either top die or bottom die
SA_CONTENT Move1(SA_CONTENT SA_contentPtr, int *moveFlag){
	//unpack the SA_contentPtr
	Die top_die = SA_contentPtr.top_die;
	Die bottom_die = SA_contentPtr.bottom_die;
	vector <Tech_menu> TechMenu = SA_contentPtr.TechMenu;
	vector <RawNet> rawnet = SA_contentPtr.rawnet;
	vector <int> PartitionResult = SA_contentPtr.PartitionResult;
	vector <Instance> InstanceArray = SA_contentPtr.InstanceArray;
	TopBottomCellArray ArrayInfo = SA_contentPtr.ArrayInfo;
	vector <int> PartitionIndexResult = ArrayInfo.PartitionIndexResult;

	int random_instance = getIntRandom(0, ArrayInfo.TopCellNumber + ArrayInfo.BottomCellNumber-1);						//choose a instance to move
	int random_instance_partition_result = PartitionResult[random_instance];											//check where is the partition of this instance
	int index = PartitionIndexResult[random_instance];																	//check the instance data is store in which index

	//the instance current placement info
	int left_edge = 0;
	int right_edge = 0;
	int rowID = 0;
	Die currentDie;

	if(random_instance_partition_result == 0){
		*moveFlag = 0;
		left_edge = ArrayInfo.BottomCellArray[index].left_edge;
		right_edge = ArrayInfo.BottomCellArray[index].right_edge;
		rowID = ArrayInfo.BottomCellArray[index].rowID;
		currentDie = bottom_die;
	}

	else if(random_instance_partition_result == 1){
		*moveFlag = 1;
		left_edge = ArrayInfo.TopCellArray[index].left_edge;
		right_edge = ArrayInfo.TopCellArray[index].right_edge;
		rowID = ArrayInfo.TopCellArray[index].rowID;
		currentDie = top_die;
	}

	//remove the segment in placement state
    for(int i=left_edge; i<=right_edge; i++){
        currentDie.PlacementState[rowID][i] = EMPTY_STATE;
    }

    //find some place to place the instance in random row
	int random_rowID = getIntRandom(0, currentDie.repeatCount-1);
	vector <point> ValidPlacementList;

	int length = right_edge - left_edge;
	left_edge = 0;
	right_edge = 0 + length;
	int listCount = 0;
	for(int i=0; i < currentDie.rowLength && right_edge < currentDie.rowLength; i++){
		if(SA_isValidPlacement(currentDie.PlacementState, random_rowID, left_edge, right_edge)){
			point temp;
			temp.x_cor = left_edge;
			temp.y_cor = right_edge;
			temp.comeFrom = 0;
			ValidPlacementList.emplace_back(temp);
			listCount++;
			if(listCount == 2) break;
		}
		int colSpacing = getIntRandom(0,currentDie.rowLength-1-length);
		left_edge += colSpacing;
		left_edge %= currentDie.rowLength;
		right_edge = left_edge + length;
	}

	SA_CONTENT new_SA_contentPtr;

	//choose a random placement in the ValidPlacementList
	if((int)ValidPlacementList.size() == 0){
		*moveFlag = -1;
		return new_SA_contentPtr;
	}
	else{
		//printf("\n**********************************************************************************************\n");
		printf("choose random instance = %d, and random rowID = %d\n", random_instance, random_rowID);
		//printPlacementState(currentDie,0);
		int random_vector_index = getIntRandom(0, (int)ValidPlacementList.size()-1);
		left_edge = ValidPlacementList[random_vector_index].x_cor;
		right_edge = ValidPlacementList[random_vector_index].y_cor;
		

		//fill the segment
		for(int i = left_edge; i <= right_edge; i++){
			currentDie.PlacementState[random_rowID][i] = random_instance;
		}

		if(random_instance_partition_result == 0){
			bottom_die.PlacementState = currentDie.PlacementState;
		}
		else{
			top_die.PlacementState = currentDie.PlacementState;
		}

		//update the cellarray and rowID
		if(random_instance_partition_result == 0){
			//printf("left edge = %d, right_edge = %d\n", left_edge, right_edge);
			ArrayInfo.BottomCellArray[index].left_edge = left_edge;
			ArrayInfo.BottomCellArray[index].right_edge = right_edge;
			ArrayInfo.BottomCellArray[index].rowID = random_rowID;
			//printf("new left edge = %d, new right edge = %d\n", ArrayInfo.BottomCellArray[index].left_edge, ArrayInfo.BottomCellArray[index].right_edge);
		}
		else if(random_instance_partition_result == 1){
			//printf("left edge = %d, right_edge = %d\n", left_edge, right_edge);
			ArrayInfo.TopCellArray[index].left_edge = left_edge;
			ArrayInfo.TopCellArray[index].right_edge = right_edge;
			ArrayInfo.TopCellArray[index].rowID = random_rowID;	
			//printf("new left edge = %d, new right edge = %d\n", ArrayInfo.TopCellArray[index].left_edge, ArrayInfo.TopCellArray[index].right_edge);		
		}

		new_SA_contentPtr.top_die = top_die;
		new_SA_contentPtr.bottom_die = bottom_die;
		new_SA_contentPtr.TechMenu = TechMenu;
		new_SA_contentPtr.rawnet = rawnet;
		new_SA_contentPtr.PartitionResult = PartitionResult;
		new_SA_contentPtr.InstanceArray = InstanceArray;
		new_SA_contentPtr.ArrayInfo = ArrayInfo;
		new_SA_contentPtr.ArrayInfo.TopCellArray = ArrayInfo.TopCellArray;
		new_SA_contentPtr.ArrayInfo.BottomCellArray = ArrayInfo.BottomCellArray;

		//printPlacementState(currentDie, 0);
		//printf("**********************************************************************************************\n");
		return new_SA_contentPtr; 
	}
}

//f(delta c, T) = min(1, exp(-delta C / T))
double f(int delta_cost, double Temperature){
	double temp = -1 * delta_cost / Temperature;
	printf("temp = %lf\n", temp);
	printf("exp(temp) = %lf\n", exp(temp));
	return 1 > exp(temp) ? exp(temp) : 1;
}

bool accept(unsigned long long int new_cost, unsigned long long int old_cost, double Temperature){
	int delta_cost = new_cost - old_cost;
	double y = f(delta_cost, Temperature);
	double r = getDoubleRandom();

	if(r < y) return true;
	else return 0;
}

SA_CONTENT SimulateAnnealing(SA_CONTENT SA_contentPtr, double ANNEALING_TEMPERATURE, double TERMINATE_TEMPERATURE, int INNER_LOOP_TIMES, double ALPHA){

	//open a file to record the cost
	FILE *costOUT = fopen("cost.out", "w");

	double Temperature = ANNEALING_TEMPERATURE;
	SA_CONTENT new_SA_contentPtr;
	int breakCount = 0;

	bool canBreak = false;
	while(Temperature > TERMINATE_TEMPERATURE){
		printf(" I am now goint to simulate annealing\n");
		if(canBreak) return SA_contentPtr;
		unsigned long long int old_cost = 0;
		unsigned long long int new_cost = 0;
		for(int i = 0; i < INNER_LOOP_TIMES; i++){
			printf("%g sec pass...\n", omp_get_wtime()-start_time);
			if(omp_get_wtime() - start_time > CUT_OFF_TIME){
				printf(">>>>>END!!!%g %g\n", omp_get_wtime(), start_time);
				canBreak = true;
				break;
			}
			if(breakCount >= INNER_LOOP_TIMES * 10) return SA_contentPtr;			//early return if converges
			int moveFlag;
			new_SA_contentPtr = Move1(SA_contentPtr, &moveFlag);
			//printPlacementState(new_SA_contentPtr.top_die,0);
			//printPlacementState(new_SA_contentPtr.bottom_die,0);

			if(moveFlag == -1){
				printf("No such place to place!\n");
				printf("Denied!\n");
				printf("End\n\n");
			}

			if(moveFlag != -1){
				old_cost = Cost(SA_contentPtr);
				new_cost = Cost(new_SA_contentPtr);
				if(old_cost == new_cost) breakCount++;
				printf("old_cost = %lld, new_cost = %lld\n",old_cost, new_cost);
				printf("at temperature = %f, INNER_LOOP_TIMES = %d\n", Temperature, i);

				if(accept(new_cost, old_cost, Temperature)){

					if(new_cost != old_cost) breakCount = 0;
					printf("Accepted!\n");

					if(moveFlag == 0){
						SA_contentPtr.bottom_die = new_SA_contentPtr.bottom_die;
						SA_contentPtr.ArrayInfo = new_SA_contentPtr.ArrayInfo;
						printf("End\n\n");
					}
					else if(moveFlag == 1){
						SA_contentPtr.top_die = new_SA_contentPtr.top_die;
						SA_contentPtr.ArrayInfo = new_SA_contentPtr.ArrayInfo;
						printf("End\n\n");
					}
				}
				else{
					printf("Denied!\n");
					printf("End\n\n");
				}
			}
			//OutputCellLocateState(SA_contentPtr.ArrayInfo, SA_contentPtr.top_die, SA_contentPtr.bottom_die, SA_contentPtr.rawnet, SA_contentPtr.TechMenu, SA_contentPtr.PartitionResult, SA_contentPtr.InstanceArray);
		}
		fprintf(costOUT, "%lld,",old_cost);
		Temperature = (Temperature > 1) ? Temperature * 0.1 : Temperature * 0.95;
	}
	fclose(costOUT);
	return SA_contentPtr;
}

void StoreBBOX(SA_CONTENT SA_contentPtr, vector <Net> &NetArray){

	//unpack the SA_contentPtr
	Die top_die = SA_contentPtr.top_die;
	Die bottom_die = SA_contentPtr.bottom_die;
	vector <Tech_menu> TechMenu = SA_contentPtr.TechMenu;
	vector <RawNet> rawnet = SA_contentPtr.rawnet;
	vector <int> PartitionResult = SA_contentPtr.PartitionResult;
	vector <Instance> InstanceArray = SA_contentPtr.InstanceArray;
	TopBottomCellArray ArrayInfo = SA_contentPtr.ArrayInfo;


    vector <bbox> bboxes;

    //output the pin location
    for(int i = 0; i < (int)rawnet.size(); i++){

        //the record for the bounding box
        int x_min = INT_MAX;
        int x_max = -1;
        int y_min = INT_MAX;
        int y_max = -1;

        //find the pin location that rawnet[i] connect to
        for(int j=0; j <rawnet[i].numPins; j++){
            char buffer[INSTANCE_NAME_SIZE];
            memset(buffer,'\0',INSTANCE_NAME_SIZE);
            char PinName[PIN_NAME_SIZE];
            memset(PinName,'\0',PIN_NAME_SIZE);
            strncpy(PinName, &rawnet[i].Connection[j].libPinName[1], strlen(rawnet[i].Connection[j].libPinName)-1);
            strncpy(buffer, &rawnet[i].Connection[j].instName[1], strlen(rawnet[i].Connection[j].instName)-1);
            int current_instance = atoi(buffer);

            //check the instance belongs to which die
            int current_instance_partition = PartitionResult[current_instance-1];
            int index = ArrayInfo.PartitionIndexResult[current_instance-1];
            int reference_left_edge = 0;
            int reference_row = 0;
            char current_tech[TECH_NAME_SIZE];
            memset(current_tech,'\0',TECH_NAME_SIZE);
            char libCellName[LIBCELL_NAME_SIZE];
            memset(libCellName,'\0',LIBCELL_NAME_SIZE);

            if(current_instance_partition == 0){
                reference_left_edge = ArrayInfo.BottomCellArray[index].left_edge;
                reference_row = ArrayInfo.BottomCellArray[index].rowID * bottom_die.rowHeight;
                strcpy(current_tech, bottom_die.tech);
                strncpy(libCellName, &InstanceArray[current_instance-1].libCellName[2], strlen(InstanceArray[current_instance-1].libCellName)-2);
            }

            else if(current_instance_partition == 1){
                reference_left_edge = ArrayInfo.TopCellArray[index].left_edge;
                reference_row = ArrayInfo.TopCellArray[index].rowID * top_die.rowHeight;
                strcpy(current_tech, top_die.tech);
                strncpy(libCellName, &InstanceArray[current_instance-1].libCellName[2], strlen(InstanceArray[current_instance-1].libCellName)-2);
            }

            for(int k=0; k < (int)TechMenu.size(); k++){
                if(strcmp(current_tech, TechMenu[k].tech) == 0){
                	x_min = (reference_left_edge + TechMenu[k].libcell[atoi(libCellName)-1].pinarray[atoi(PinName)-1].pinLocationX) < x_min ? (reference_left_edge + TechMenu[k].libcell[atoi(libCellName)-1].pinarray[atoi(PinName)-1].pinLocationX) : x_min ;
                	x_max = (reference_left_edge + TechMenu[k].libcell[atoi(libCellName)-1].pinarray[atoi(PinName)-1].pinLocationX) > x_max ? (reference_left_edge + TechMenu[k].libcell[atoi(libCellName)-1].pinarray[atoi(PinName)-1].pinLocationX) : x_max ;
					y_min = (reference_row + TechMenu[k].libcell[atoi(libCellName)-1].pinarray[atoi(PinName)-1].pinLocationY) < y_min ? (reference_row + TechMenu[k].libcell[atoi(libCellName)-1].pinarray[atoi(PinName)-1].pinLocationY) : y_min;
					y_max = (reference_row + TechMenu[k].libcell[atoi(libCellName)-1].pinarray[atoi(PinName)-1].pinLocationY) > y_max ? (reference_row + TechMenu[k].libcell[atoi(libCellName)-1].pinarray[atoi(PinName)-1].pinLocationY) : y_max;
                }
            }            

        }
        NetArray[i].x_min = x_min;
        NetArray[i].x_max = x_max;
        NetArray[i].y_min = y_min;
        NetArray[i].y_max = y_max;
    }
}

void printBBOX(vector <Net> NetArray){
	for(int i = 0; i < (int)NetArray.size(); i++){
		printf("Net %d: <%d %d %d %d>\n", i+1, NetArray[i].x_min, NetArray[i].x_max, NetArray[i].y_min, NetArray[i].y_max);
	}
}