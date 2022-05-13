#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>
#include "placement.h"
#include "readfile.h"
#include "partition.h"

bool isValidPlacement(vector <vector <int>> PlacementState, int row, int left_edge, int right_edge){
    
    for(int i=left_edge; i<=right_edge; i++){
        // printf("%d ",i);
        if(PlacementState[row][i]!=EMPTY_STATE){
            // printf("\n");
            return false;
        }
    }
    // printf(" \n");
    return true;
}

void fillSegment(vector <vector <int>> &PlacementState, int row, int left_pointer, int right_pointer, int cellID){

    for(int i=left_pointer; i<=right_pointer; i++){
        PlacementState[row][i]=cellID;
    }
}

//if flag = 0, place the bottom die, if flag = 1, place the top die
void InitializePlacement(Die *currentDie, TopBottomCellArray *ArrayInfo, int flag, bool *PartitionAgain){
    *PartitionAgain = true;
    int usingCell = 0;
    vector <vector <int>> PlacementState(currentDie->repeatCount,vector <int>(currentDie->rowLength,EMPTY_STATE));
    int sucessful_placement_count = 0;
    if(flag == 0){
        for( int i=0; i< (*ArrayInfo).BottomCellNumber; i++){
            int row=0;
            int left_edge = currentDie->startX;
            int right_edge = left_edge + (*ArrayInfo).BottomCellArray[i].libCellSizeX-1 ;
            while( row < currentDie->rowLength){
                //find the continue segmenet to place instance
                if( isValidPlacement(PlacementState, row, left_edge, right_edge)){
                    fillSegment(PlacementState, row, left_edge, right_edge, (*ArrayInfo).BottomCellArray[i].cellID);
                    (*ArrayInfo).BottomCellArray[i].rowID = row;
                    (*ArrayInfo).BottomCellArray[i].left_edge = left_edge;
                    (*ArrayInfo).BottomCellArray[i].right_edge = right_edge;
                    sucessful_placement_count++;
                    usingCell += right_edge - left_edge + 1;
                    break;
                }
                else{
                    left_edge++;
                    right_edge++;
                }
                if( right_edge >= currentDie->rowLength ){
                    row++;
                    if(row >= currentDie->repeatCount){
                        return;
                    }
                    left_edge = currentDie->startX;
                    right_edge = left_edge + (*ArrayInfo).BottomCellArray[i].libCellSizeX-1;
                }
            }
        }
        if(sucessful_placement_count == (*ArrayInfo).BottomCellNumber) *PartitionAgain = false;
        printf("successful placement count : %d \n",sucessful_placement_count);

        printf("MaxUtil of bottom die: %d Current Partition Util: %d\n\n", currentDie->MaxUtil, usingCell * 100/currentDie->rowLength /currentDie->repeatCount);
        if(usingCell * 100/currentDie->rowLength /currentDie->repeatCount > currentDie->MaxUtil) *PartitionAgain = true;
    }
    else if(flag == 1){
        for( int i=0; i<(*ArrayInfo).TopCellNumber; i++){
            int row=0;
            int left_edge = currentDie->startX;
            int right_edge = left_edge + (*ArrayInfo).TopCellArray[i].libCellSizeX-1 ;
            while( row < currentDie->rowLength){
                //find the continue segmenet to place instance
                if( isValidPlacement(PlacementState, row, left_edge, right_edge)){
                    fillSegment(PlacementState, row, left_edge, right_edge, (*ArrayInfo).TopCellArray[i].cellID);
                    (*ArrayInfo).TopCellArray[i].rowID = row;
                    (*ArrayInfo).TopCellArray[i].left_edge = left_edge;
                    (*ArrayInfo).TopCellArray[i].right_edge = right_edge;
                    sucessful_placement_count++;
                    usingCell += right_edge - left_edge + 1;
                    break;
                }
                else{
                    left_edge++;
                    right_edge++;
                }
                if( right_edge >= currentDie->rowLength ){
                    row++;
                    if(row >= currentDie->repeatCount){
                        return;
                    }
                    left_edge = currentDie->startX;
                    right_edge = left_edge + (*ArrayInfo).TopCellArray[i].libCellSizeX-1;
                }
            }
        }
        if(sucessful_placement_count == (*ArrayInfo).TopCellNumber) *PartitionAgain = false;
        printf("successful placement count : %d \n",sucessful_placement_count);

        printf("MaxUtil of top die: %d Current Partition Util: %d\n\n", currentDie->MaxUtil, usingCell * 100/currentDie->rowLength /currentDie->repeatCount);
        if(usingCell * 100/currentDie->rowLength /currentDie->repeatCount > currentDie->MaxUtil) *PartitionAgain = true;
    }
    else{
        assert(flag!=0 || flag!=1);
    }
    currentDie->PlacementState = PlacementState;
}

void printPlacementState(Die current_die, bool PartitionAgain){
    if(PartitionAgain ==0){
        for(int i=0; i<(int)current_die.PlacementState.size(); i++){
            for(int j=0; j<(int)current_die.PlacementState[i].size(); j++){
                printf("%2d ", current_die.PlacementState[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
    else if(PartitionAgain == 1){
        printf("go eat garbage!!!!!!!!!!!!!!!!!!!!!!!!\n");
    }
    else{
        assert(PartitionAgain != 0 || PartitionAgain != 1);
    }
}

void OutputCellLocateState(TopBottomCellArray ArrayInfo, Die top_die, Die bottom_die, vector <RawNet> rawnet, vector <Tech_menu> TechMenu, vector <int> PartitionResult, vector <Instance> InstanceArray){
    FILE *placementDraw = fopen("placement.txt", "w");
    fprintf(placementDraw, "%d %d %d\n", bottom_die.repeatCount, bottom_die.rowLength, bottom_die.rowHeight);
    fprintf(placementDraw, "%d\n", ArrayInfo.BottomCellNumber);
    for(int i = 0; i < ArrayInfo.BottomCellNumber; i++){
        fprintf(placementDraw, "%d %d %d %d\n", ArrayInfo.BottomCellArray[i].cellID, ArrayInfo.BottomCellArray[i].rowID, ArrayInfo.BottomCellArray[i].left_edge, ArrayInfo.BottomCellArray[i].right_edge);
    }

    fprintf(placementDraw, "%d %d %d\n", top_die.repeatCount, top_die.rowLength, top_die.rowHeight);
    fprintf(placementDraw, "%d\n", ArrayInfo.TopCellNumber);
    for(int i = 0; i < ArrayInfo.TopCellNumber; i++){
        fprintf(placementDraw, "%d %d %d %d\n", ArrayInfo.TopCellArray[i].cellID, ArrayInfo.TopCellArray[i].rowID, ArrayInfo.TopCellArray[i].left_edge, ArrayInfo.TopCellArray[i].right_edge);
    }

    vector <vector <point>> pinLocationPairs;

    //output the pin location
    for(int i = 0; i < (int)rawnet.size(); i++){
        vector <point> pinLocationPair;


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
                                        point temp;
                                        temp.x_cor = reference_left_edge + TechMenu[k].libcell[l].pinarray[m].pinLocationX;
                                        temp.y_cor = reference_row + TechMenu[k].libcell[l].pinarray[m].pinLocationY;
                                        temp.comeFrom = 0;
                                        pinLocationPair.emplace_back(temp);
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
                                        point temp;
                                        temp.x_cor = reference_left_edge + TechMenu[k].libcell[l].pinarray[m].pinLocationX;
                                        temp.y_cor = reference_row + TechMenu[k].libcell[l].pinarray[m].pinLocationY;
                                        temp.comeFrom = 1;
                                        pinLocationPair.emplace_back(temp);
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
        pinLocationPairs.emplace_back(pinLocationPair);
    }

    //start to output the point that connect in same nets
    fprintf(placementDraw, "%d\n", (int)pinLocationPairs.size());
    for(int i = 0 ; i < (int)pinLocationPairs.size(); i++){
        fprintf(placementDraw, "%d\n", (int)pinLocationPairs[i].size());
        for(int j = 0 ; j < (int)pinLocationPairs[i].size(); j++){
            fprintf(placementDraw, "%d %d %d\n", pinLocationPairs[i][j].x_cor, pinLocationPairs[i][j].y_cor, pinLocationPairs[i][j].comeFrom);
        }
    }

    fclose(placementDraw);
}