#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
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
void InitializePlacement(Die *currentDie, TopBottomCellArray ArrayInfo, int flag, bool *PartitionAgain){
    *PartitionAgain = true;
    vector <vector <int>> PlacementState(currentDie->repeatCount,vector <int>(currentDie->rowLength,EMPTY_STATE));
    int sucessful_placement_count = 0;
    if(flag == 0){
        for( int i=0; i<ArrayInfo.BottomCellNumber; i++){
            int row=0;
            int left_edge = currentDie->startX;
            int right_edge = left_edge + ArrayInfo.BottomCellArray[i].libCellSizeX-1 ;
            while( row < currentDie->rowLength){
                //find the continue segmenet to place instance
                if( isValidPlacement(PlacementState, row, left_edge, right_edge)){
                    fillSegment(PlacementState, row, left_edge, right_edge, ArrayInfo.BottomCellArray[i].cellID);
                    sucessful_placement_count++;
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
                    right_edge = left_edge + ArrayInfo.BottomCellArray[i].libCellSizeX-1;
                }
            }
        }
        if(sucessful_placement_count == ArrayInfo.BottomCellNumber) {
            *PartitionAgain = false;
        }
        printf("successful placement count : %d \n",sucessful_placement_count);
    
    }
    else if(flag == 1){
        for( int i=0; i<ArrayInfo.TopCellNumber; i++){
            int row=0;
            int left_edge = currentDie->startX;
            int right_edge = left_edge + ArrayInfo.TopCellArray[i].libCellSizeX-1 ;
            while( row < currentDie->rowLength){
                //find the continue segmenet to place instance
                if( isValidPlacement(PlacementState, row, left_edge, right_edge)){
                    fillSegment(PlacementState, row, left_edge, right_edge, ArrayInfo.TopCellArray[i].cellID);
                    sucessful_placement_count++;
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
                    right_edge = left_edge + ArrayInfo.TopCellArray[i].libCellSizeX-1;
                }
            }
        }
        if(sucessful_placement_count == ArrayInfo.TopCellNumber) *PartitionAgain = false;
        printf("successful placement count : %d \n",sucessful_placement_count);
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