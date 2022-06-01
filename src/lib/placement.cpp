#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>
#include "placement.h"
#include "readfile.h"
#include "partition.h"

bool isValidPlacement(vector <int> &NewLeftEdgeArray, int currentRow, int left_edge, int right_edge, int rowLength){
    
    if(right_edge <= rowLength-1){
        return true;
    }
    else return false;
}

void fillSegment(vector <vector <int>> &PlacementState, int row, int left_pointer, int right_pointer, int cellID){

    for(int i=left_pointer; i<=right_pointer; i++){
        PlacementState[row][i]=cellID;
    }
}

//if flag = 0, place the bottom die, if flag = 1, place the top die
void InitializePlacement(Die *currentDie, TopBottomCellArray *ArrayInfo, int flag, bool *PartitionAgain){
    *PartitionAgain = true;
    int currentCellNumber = 0;
    vector <Cell> currentCellArray;

    //calculate the sum of the cells' size

    if(flag == 0){
        currentCellNumber = (*ArrayInfo).BottomCellNumber;
        currentCellArray = (*ArrayInfo).BottomCellArray;
    }
    else if(flag == 1){
        currentCellNumber = (*ArrayInfo).TopCellNumber;
        currentCellArray = (*ArrayInfo).TopCellArray;
    }
    else{
        assert(flag!=0 || flag!=1);
    }

    double SumOfCellLength = 0;
    double MaxLength = (currentDie->rowLength * currentDie->repeatCount);

    for(int i=0; i < currentCellNumber; i++){
        SumOfCellLength += currentCellArray[i].libCellSizeX;
    }

    if( SumOfCellLength / MaxLength * 100 >= (double)currentDie->MaxUtil){
        if(flag==0) printf("Bottom Die: Violate the MaxUtil -> Partition again!\n");
        else printf("Top Die: Violate the MaxUtil -> Partition again!\n");
        printf("<test for MaxUtil> MaxUtil of current die: %d Current Partition Util: %lf \n\n", currentDie->MaxUtil, SumOfCellLength / MaxLength * 100);
        return;
    }


    //start to place the instance
    unsigned long long int usingCell = 0;
    vector <vector <int>> PlacementState(currentDie->repeatCount,vector <int>(currentDie->rowLength,EMPTY_STATE));
    int sucessful_placement_count = 0;

    //linear probing 
    int currentRow = 0;
    int left_edge = 0;
    int right_edge;
    int rowLength = currentDie->rowLength;
    vector <int> NewLeftEdgeArray(currentDie->repeatCount,0);

    for(int i=0; i < currentCellNumber; i++){

        for(int j=0; j<currentDie->repeatCount; j++){
            int rowSpacing = (currentDie->repeatCount % 2 == 0) ? 3 : 2;
            currentRow = (currentRow+j + rowSpacing) % currentDie->repeatCount;      //current+1+j ?
            left_edge = NewLeftEdgeArray[currentRow];
            right_edge = left_edge + currentCellArray[i].libCellSizeX-1;
            if(isValidPlacement(NewLeftEdgeArray, currentRow, left_edge, right_edge, rowLength)){
                fillSegment(PlacementState, currentRow, left_edge, right_edge, currentCellArray[i].cellID);
                currentCellArray[i].rowID = currentRow;
                currentCellArray[i].left_edge = left_edge;
                currentCellArray[i].right_edge = right_edge;
                sucessful_placement_count++;
                usingCell += currentCellArray[i].libCellSizeX;
                NewLeftEdgeArray[currentRow] = right_edge+1;
                break; 
            }
        }
    }

    if(sucessful_placement_count == currentCellNumber) *PartitionAgain = false;
    printf("MaxUtil of current die: %d Current Partition Util: %d \n\n", currentDie->MaxUtil, int(usingCell *100/rowLength / currentDie->repeatCount));
    if(int(usingCell * 100/currentDie->rowLength / currentDie->repeatCount) > currentDie->MaxUtil-1) *PartitionAgain = true;

    //pack data
    if(flag == 0){
        (*ArrayInfo).BottomCellArray = currentCellArray;
    }
    else if(flag == 1){
        (*ArrayInfo).TopCellArray = currentCellArray;
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
    vector <bbox> bboxes;

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
            memset(buffer,'\0',INSTANCE_NAME_SIZE);
            char PinName[PIN_NAME_SIZE];
            memset(PinName, '\0', PIN_NAME_SIZE);
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
                memset(current_tech,'\0', TECH_NAME_SIZE);
                strcpy(current_tech, bottom_die.tech);
                char libCellName[LIBCELL_NAME_SIZE];
                memset(libCellName,'\0',LIBCELL_NAME_SIZE);
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
                                        if(temp.x_cor >= reference_left_edge + ArrayInfo.BottomCellArray[index].right_edge - ArrayInfo.BottomCellArray[index].left_edge + 1){
                                            // printf("\n%d\n", current_instance);
                                            // printf("pinLocation: (%d,%d) \n", TechMenu[k].libcell[l].pinarray[m].pinLocationX, TechMenu[k].libcell[l].pinarray[m].pinLocationY);
                                            // printf("%d %d\n", ArrayInfo.BottomCellArray[index].right_edge, ArrayInfo.BottomCellArray[index].left_edge);
                                            // printf("libCellSizeX: %d\n", ArrayInfo.BottomCellArray[index].libCellSizeX);
                                            // printf("cellID = %d\n", ArrayInfo.BottomCellArray[index].cellID);
                                        }
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
                memset(current_tech,'\0', TECH_NAME_SIZE);
                strcpy(current_tech, top_die.tech);
                char libCellName[LIBCELL_NAME_SIZE];
                memset(libCellName,'\0',LIBCELL_NAME_SIZE);
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
                                        if(temp.x_cor >= reference_left_edge + ArrayInfo.TopCellArray[index].right_edge - ArrayInfo.TopCellArray[index].left_edge + 1){
                                            // printf("\n%d\n", current_instance);
                                            // printf("pinLocation: (%d,%d) \n", TechMenu[k].libcell[l].pinarray[m].pinLocationX, TechMenu[k].libcell[l].pinarray[m].pinLocationY);
                                            // printf("%d %d\n", ArrayInfo.TopCellArray[index].right_edge, ArrayInfo.TopCellArray[index].left_edge);
                                            // printf("libCellSizeX: %d\n", ArrayInfo.TopCellArray[index].libCellSizeX);
                                            // printf("cellID = %d\n", ArrayInfo.TopCellArray[index].cellID);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        pinLocationPairs.emplace_back(pinLocationPair);
        bbox temp_bbox;
        temp_bbox.x_min = x_min;
        temp_bbox.x_max = x_max;
        temp_bbox.y_min = y_min;
        temp_bbox.y_max = y_max;
        bboxes.emplace_back(temp_bbox); 
    }

    //start to output the point that connect in same nets
    fprintf(placementDraw, "%d\n", (int)pinLocationPairs.size());
    for(int i = 0 ; i < (int)pinLocationPairs.size(); i++){
        fprintf(placementDraw, "%d\n", (int)pinLocationPairs[i].size());
        for(int j = 0 ; j < (int)pinLocationPairs[i].size(); j++){
            fprintf(placementDraw, "%d %d %d\n", pinLocationPairs[i][j].x_cor, pinLocationPairs[i][j].y_cor, pinLocationPairs[i][j].comeFrom);
        }
    }

    for(int i = 0; i < (int)bboxes.size(); i++){
        fprintf(placementDraw, "%d %d %d %d\n", bboxes[i].x_min, bboxes[i].x_max, bboxes[i].y_min, bboxes[i].y_max);
    }

    fclose(placementDraw);
}


void printArrayInfo(TopBottomCellArray *ArrayInfo){

    printf("BottomCell Number: %d\n", ArrayInfo->BottomCellNumber);
    for(int i = 0; i < ArrayInfo->BottomCellNumber; i++){
        printf("\tCell ID: %d\n", ArrayInfo->BottomCellArray[i].cellID);
        printf("\t\tBelongs to %d Die\n", ArrayInfo->BottomCellArray[i].WhichDie);
        printf("\t\tRowID = %d, left_edge = %d, right_edge = %d\n", ArrayInfo->BottomCellArray[i].rowID, ArrayInfo->BottomCellArray[i].left_edge, ArrayInfo->BottomCellArray[i].right_edge);
        printf("\t\t(libCellSizeX, libCellSizeY) = (%d, %d)\n", ArrayInfo->BottomCellArray[i].libCellSizeX, ArrayInfo->BottomCellArray[i].libCellSizeY);
        printf("\n");
    }

    printf("TopCell Number: %d\n", ArrayInfo->TopCellNumber);
    for(int i = 0; i < ArrayInfo->TopCellNumber; i++){
        printf("\tCell ID: %d\n", ArrayInfo->TopCellArray[i].cellID);
        printf("\t\tBelongs to %d Die\n", ArrayInfo->TopCellArray[i].WhichDie);
        printf("\t\tRowID = %d, left_edge = %d, right_edge = %d\n", ArrayInfo->TopCellArray[i].rowID, ArrayInfo->TopCellArray[i].left_edge, ArrayInfo->TopCellArray[i].right_edge);
        printf("\t\t(libCellSizeX, libCellSizeY) = (%d, %d)\n", ArrayInfo->TopCellArray[i].libCellSizeX, ArrayInfo->TopCellArray[i].libCellSizeY);
        printf("\n");
    }
}

bool isValidHybridPlacement(int center_x, int center_y, int terminalX, int terminalY, vector <vector <int>> &HBPlacementState, int spacing){
    for(int i= center_y - terminalY/2 - spacing +1 ; i <= center_y + terminalY/2 +spacing; i++){
        for(int j= center_x - terminalX/2 - spacing +1 ; j <= center_x + terminalX/2 +spacing; j++){
            
            if(HBPlacementState[i][j]==PLACEMENT_USED ){
                // printf("(%d, %d) isValid failed.\n",j,i);
                return false;
            }
        }
    }
    return true;
}

void fillHybridPlacement(int center_x, int center_y, int terminalX, int terminalY, vector <vector <int>> &HBPlacementState){
    for(int i= center_y - terminalY/2 +1 ; i <= center_y + terminalY/2 ; i++){
        for(int j= center_x - terminalX/2 +1 ; j <= center_x + terminalX/2 ; j++){
            HBPlacementState[i][j]=PLACEMENT_USED;
        }
    }
}

void HybridPlacement(Hybrid_terminal *terminal, Die top_die, vector <Net> &NetArray){
    vector <vector <int>> HBPlacementState(top_die.upperRightY - top_die.lowerLeftY + 1,vector <int>(top_die.upperRightX - top_die.lowerLeftX + 1,EMPTY_STATE));
    int spacing = terminal->spacing;
    int dieSizeX = top_die.upperRightX;
    int dieSizeY = top_die.upperRightY;

    // //fill the BOUNDARY_INVALID between terminal and boundary
    // for(int i = 0; i < (int)HBPlacementState.size(); i++){
    //     for(int j = 0; j < (int)HBPlacementState[i].size(); j++){
    //         if(i <= spacing){
    //             HBPlacementState[i][j] =  BOUNDARY_INVALID;
    //         }
    //         else if(i >= (int)HBPlacementState.size() - spacing){
    //             HBPlacementState[i][j] =  BOUNDARY_INVALID;
    //         }
    //         else if(j <= spacing){
    //             HBPlacementState[i][j] =  BOUNDARY_INVALID;           
    //         }
    //         else if(j >= (int)HBPlacementState[i].size() - spacing){
    //             HBPlacementState[i][j] =  BOUNDARY_INVALID; 
    //         }
    //     }
    // }
    int place_count=0;

    for(int i=0; i<(int)NetArray.size(); i++){
        if(NetArray[i].hasHybridTerminal==1){
            // printf("i=%d.\n",i);
            int lower_bound = (NetArray[i].y_min  < (spacing + (terminal->sizeY/2))) ? spacing + (terminal->sizeY/2) : NetArray[i].y_min;
            int upper_bound = (NetArray[i].y_max > dieSizeY - spacing - terminal->sizeY/2 -1 ) ? dieSizeY - spacing -1 - (terminal->sizeY/2): NetArray[i].y_max;           
            int left_bound =  (NetArray[i].x_min < spacing + (terminal->sizeX/2)) ? spacing + (terminal->sizeX/2) : NetArray[i].x_min  ;
            int right_bound = (NetArray[i].x_max > dieSizeX  - spacing - terminal->sizeX/2 -1) ? dieSizeX  - spacing -1 - (terminal->sizeX/2): NetArray[i].x_max;

        //    printf("lower bound = %d. upper= %d. left = %d. right = %d.",lower_bound, upper_bound, left_bound, right_bound);
            bool successful_placed =0;
            for(int j=lower_bound ; j <= upper_bound; j++){
                
                for(int k=left_bound; k<=right_bound; k++){
                    // printf("j = %d k = %d\n", j, k);
                    if(isValidHybridPlacement(j,k,terminal->sizeX,terminal->sizeY ,HBPlacementState, spacing)==1){
                        fillHybridPlacement(j,k,terminal->sizeX,terminal->sizeY ,HBPlacementState);
                        // printf("net %d successful putting terminal!!!\n", i+1);
                        NetArray[i].HBlocationX = k;
                        NetArray[i].HBLocationY = j;
                        successful_placed = 1;
                        place_count++;
                        break;
                    }
                }
                if(successful_placed == 1) break;
            }
            if(successful_placed != 1){
                printf("net %d failed.\n",i);
            }
        }
    }

    terminal->HBPlacementState = HBPlacementState;
    // for(int i = 0; i < (int)terminal->HBPlacementState.size(); i++){
    //     for(int j = 0; j < (int)terminal->HBPlacementState[i].size(); j++){
    //         printf("%d ", terminal->HBPlacementState[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    printf("placement count =  %d\n",place_count);

}


void outputAnswer(char *filename, TopBottomCellArray ArrayInfo, Die top_die, Die bottom_die, int NumTerminal, vector <Net> NetArray){
    FILE *out = fopen(filename, "w");
    fprintf(out, "TopDiePlacement %d\n", ArrayInfo.TopCellNumber);
    for(int i = 0; i < ArrayInfo.TopCellNumber; i++){
        fprintf(out, "Inst C%d %d %d\n", ArrayInfo.TopCellArray[i].cellID + 1, ArrayInfo.TopCellArray[i].left_edge, ArrayInfo.TopCellArray[i].rowID * top_die.rowHeight);
    }

    fprintf(out, "BottomDiePlacement %d\n", ArrayInfo.BottomCellNumber);
    for(int i = 0; i < ArrayInfo.BottomCellNumber; i++){
        fprintf(out, "Inst C%d %d %d\n", ArrayInfo.BottomCellArray[i].cellID + 1, ArrayInfo.BottomCellArray[i].left_edge, ArrayInfo.BottomCellArray[i].rowID * bottom_die.rowHeight);
    }

    fprintf(out, "NumTerminals %d\n", NumTerminal);
    for(int i = 0; i < (int)NetArray.size(); i++){
        if(NetArray[i].hasHybridTerminal){
            fprintf(out,"Terminal N%d %d %d\n", NetArray[i].NetID+1, NetArray[i].HBlocationX, NetArray[i].HBLocationY);
        }
    }

    fclose(out);
}