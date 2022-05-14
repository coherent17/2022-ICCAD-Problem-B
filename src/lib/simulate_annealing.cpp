#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>
#include <cmath>
#include "placement.h"
#include "readfile.h"
#include "partition.h"
#include "random.h"


//f(delta c, T) = min(1, exp(-delta C / T))
double f(int delta_cost, double Temperature){
	return 1 > exp(-1 * delta_cost / Temperature) ? exp(-1 * delta_cost / Temperature) : 1;
}

bool accept(int new_cost, int old_cost, double Temperature){
	int delta_cost = new_cost - old_cost;
	double y = f(delta_cost, Temperature);
	double r = getDoubleRandom();

	if(r < y) return true;
	else return 0;
}

int Cost(TopBottomCellArray ArrayInfo, Die top_die, Die bottom_die, vector <RawNet> rawnet, vector <Tech_menu> TechMenu, vector <int> PartitionResult, vector <Instance> InstanceArray){
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
            memset(buffer,0,INSTANCE_NAME_SIZE);
            char PinName[PIN_NAME_SIZE];
            strcpy(PinName, rawnet[i].Connection[j].libPinName);
            strncpy(buffer, &rawnet[i].Connection[j].instName[1], strlen(rawnet[i].Connection[j].instName)-1);
            int current_instance = atoi(buffer);

            //check the instance belongs to which die
            int current_instance_partition = PartitionResult[current_instance-1];

            if(current_instance_partition == 0){
                //get the cell placement
                int index = ArrayInfo.PartitionIndexResult[current_instance-1];
                int reference_left_edge = ArrayInfo.BottomCellArray[index].left_edge;
                int reference_row = ArrayInfo.BottomCellArray[index].rowID * bottom_die.rowHeight;

                //get the pin placement reference to this instance
                char current_tech[TECH_NAME_SIZE];
                strcpy(current_tech, bottom_die.tech);
                char libCellName[LIBCELL_NAME_SIZE];
                strcpy(libCellName, InstanceArray[current_instance-1].libCellName);

                for(int k=0; k < (int)TechMenu.size(); k++){
                    if(strcmp(current_tech, TechMenu[k].tech) == 0){
                        for(int l=0; l < (int)TechMenu[k].libcell.size(); l++){
                            if(strcmp(libCellName,TechMenu[k].libcell[l].libCellName) == 0){
                                for(int m=0; m < (int)TechMenu[k].libcell[l].pinarray.size(); m++){
                                    if(strcmp(PinName, TechMenu[k].libcell[l].pinarray[m].pinName) == 0){
                                        x_min = (reference_left_edge + TechMenu[k].libcell[l].pinarray[m].pinLocationX) < x_min ? (reference_left_edge + TechMenu[k].libcell[l].pinarray[m].pinLocationX) : x_min ;
                                        x_max = (reference_left_edge + TechMenu[k].libcell[l].pinarray[m].pinLocationX) > x_max ? (reference_left_edge + TechMenu[k].libcell[l].pinarray[m].pinLocationX) : x_max ;
                                        y_min = (reference_row + TechMenu[k].libcell[l].pinarray[m].pinLocationY) < y_min ? (reference_row + TechMenu[k].libcell[l].pinarray[m].pinLocationY) : y_min;
                                        y_max = (reference_row + TechMenu[k].libcell[l].pinarray[m].pinLocationY) > y_max ? (reference_row + TechMenu[k].libcell[l].pinarray[m].pinLocationY) : y_max;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            else if(current_instance_partition == 1){
                //get the cell placement
                int index = ArrayInfo.PartitionIndexResult[current_instance-1];
                int reference_left_edge = ArrayInfo.TopCellArray[index].left_edge;
                int reference_row = ArrayInfo.TopCellArray[index].rowID * top_die.rowHeight;

                //get the pin placement reference to this instance
                char current_tech[TECH_NAME_SIZE];
                strcpy(current_tech, top_die.tech);
                char libCellName[LIBCELL_NAME_SIZE];
                strcpy(libCellName, InstanceArray[current_instance-1].libCellName);

                for(int k=0; k < (int)TechMenu.size(); k++){
                    if(strcmp(current_tech, TechMenu[k].tech) == 0){
                        for(int l=0; l < (int)TechMenu[k].libcell.size(); l++){
                            if(strcmp(libCellName,TechMenu[k].libcell[l].libCellName) == 0){
                                for(int m=0; m < (int)TechMenu[k].libcell[l].pinarray.size(); m++){
                                    if(strcmp(PinName, TechMenu[k].libcell[l].pinarray[m].pinName) == 0){
                                        x_min = (reference_left_edge + TechMenu[k].libcell[l].pinarray[m].pinLocationX) < x_min ? (reference_left_edge + TechMenu[k].libcell[l].pinarray[m].pinLocationX) : x_min ;
                                        x_max = (reference_left_edge + TechMenu[k].libcell[l].pinarray[m].pinLocationX) > x_max ? (reference_left_edge + TechMenu[k].libcell[l].pinarray[m].pinLocationX) : x_max ;
                                        y_min = (reference_row + TechMenu[k].libcell[l].pinarray[m].pinLocationY) < y_min ? (reference_row + TechMenu[k].libcell[l].pinarray[m].pinLocationY) : y_min;
                                        y_max = (reference_row + TechMenu[k].libcell[l].pinarray[m].pinLocationY) > y_max ? (reference_row + TechMenu[k].libcell[l].pinarray[m].pinLocationY) : y_max;
                                    }
                                }
                            }
                        }
                    }
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

    int result = 0;
    //calculate the total similar_HPWL of the bbox
    for(int i = 0; i < (int)bboxes.size(); i++){
    	printf("%d\n", (bboxes[i].x_max - bboxes[i].x_min) + (bboxes[i].y_max - bboxes[i].y_min));
    	result += (bboxes[i].x_max - bboxes[i].x_min) + (bboxes[i].y_max - bboxes[i].y_min);
    }
    return result;
}


//return the placement state of either top die or bottom die
vector<vector<int>> Move1(TopBottomCellArray *CurrentArrayInfo, vector <int> PartitionResult, vector <int> PartitionIndexResult, Die top_die, Die bottom_die, TopBottomCellArray *ArrayInfo){
	int random_instance = getIntRandom(0, CurrentArrayInfo->TopCellNumber + CurrentArrayInfo->BottomCellNumber);
	int random_instance_partition_result = PartitionResult[random_instance];
	if(random_instance_partition_result == 0){
		int index = PartitionIndexResult[random_instance];
		int left_edge = CurrentArrayInfo->BottomCellArray[index].left_edge;
		int right_edge = CurrentArrayInfo->BottomCellArray[index].right_edge;
		int rowID = CurrentArrayInfo->BottomCellArray[index].rowID;

		Die currentDie = bottom_die;

		//remove the segment in placement state
	    for(int i=left_edge; i<=right_edge; i++){
	        currentDie.PlacementState[rowID][i] = EMPTY_STATE;
	    }

	    //find some place to place the instance in random row
		int random_rowID = getIntRandom(0, currentDie.repeatCount);
		vector <point> ValidPlacementList;
		for(int i=0; i < currentDie.rowLength; i++){
			if(isValidPlacement(currentDie.PlacementState, random_rowID, left_edge, right_edge)){
				point temp;
				temp.x_cor = left_edge;
				temp.y_cor = right_edge;
				temp.comeFrom = 0;
				ValidPlacementList.emplace_back(temp);
			}
		}
			
		//choose a random placement in the ValidPlacementList
		if((int)ValidPlacementList.size() == 0) return NULL;
		else{
			int random_vector_index = getIntRandom(0, (int)ValidPlacementList.size());

			//fill the segment
			for(int i = left_edge; i <= right_edge; i++){
				currentDie.PlacementState[random_rowID][i] = random_instance;
			}
			return currentDie.PlacementState; 
		}
	}


	else if(random_instance_partition_result == 1){
		int index = PartitionIndexResult[random_instance];
		int left_edge = CurrentArrayInfo->TopCellArray[index].left_edge;
		int right_edge = CurrentArrayInfo->TopCellArray[index].right_edge;

		Die currentDie = top_die;

		//remove the segment in placement state
	    for(int i=left_edge; i<=right_edge; i++){
	        currentDie.PlacementState[rowID][i] = EMPTY_STATE;
	    }

	    //find some place to place the instance in random row
		int random_rowID = getIntRandom(0, currentDie.repeatCount);
		vector <point> ValidPlacementList;
		for(int i=0; i < currentDie.rowLength; i++){
			if(isValidPlacement(currentDie.PlacementState, random_rowID, left_edge, right_edge)){
				point temp;
				temp.x_cor = left_edge;
				temp.y_cor = right_edge;
				temp.comeFrom = 0;
				ValidPlacementList.emplace_back(temp);
			}
		}
			
		//choose a random placement in the ValidPlacementList
		if((int)ValidPlacementList.size() == 0) return NULL;
		else{
			int random_vector_index = getIntRandom(0, (int)ValidPlacementList.size());

			//fill the segment
			for(int i = left_edge; i <= right_edge; i++){
				currentDie.PlacementState[random_rowID][i] = random_instance;
			}
			return currentDie.PlacementState; 
		}
	}
	return NULL;
}


void SimulateAnnealing(TopBottomCellArray *ArrayInfo){
	double Temperature = ANNEALING_TEMPERATURE;

	TopBottomCellArray CurrentArrayInfo = *ArrayInfo;
	while(Temperature > TERMINATE_TEMPERATURE){
		for(int i = 0; i < INNER_LOOP_TIMES; i++){
			if(i%2 == 0){
				Move1(&CurrentArrayInfo);
				if(accept()){
					*ArrayInfo = CurrentArrayInfo;
				}
			}
			else{
				Move2();
			}
		}
	}
}