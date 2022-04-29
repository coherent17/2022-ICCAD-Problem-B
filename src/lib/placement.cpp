#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "placement.h"
#include "readfile.h"
#include "partition.h"

bool isValidPlacement(vector <vector <int>> PlacementState, int row, int left_pointer, int right_pointer){
    
    for(int i=left_pointer; i<=right_pointer; i++){
        if(PlacementState[row][i]!=EMPTY_STATE){
            return false;
        }
    }
    return true;
}

void fillSegment(vector <vector <int>> PlacementState, int row, int left_pointer, int right_pointer, int cellID){

    for(int i=left_pointer; i<=right_pointer; i++){
        PlacementState[row][i]=cellID;
    }
}

//if flag = 0, place the bottom die, if flag = 1, place the top die
void InitializePlacement(Die *currentDie, TopBottomCellArray ArrayInfo, int flag){
    vector <vector <int>> PlacementState(currentDie->repeatCount,vector <int>(currentDie->rowLength,EMPTY_STATE));
    if(flag == 0){
        int i=0;
        int row=0;
        int left=currentDie->startX;
        while( i<(int)ArrayInfo.BottomCellArray.size()){
            printf("iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii\n");
            while ( row < currentDie->repeatCount){
                printf("rowrowrowrowrowrowrowrowrowrowrowrow\n");
                while( left<currentDie->rowLength){
                    printf("lllllllllllllllllllllllllllllllllllllllllllllllllllll\n");
                    int right=left;
                    while( right<currentDie->rowLength){
                        printf("rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr\n");
                        if( right-left+1==ArrayInfo.BottomCellArray[i].libCellSizeX && isValidPlacement(PlacementState,row, left, right)){
                            fillSegment(PlacementState, row, left, right, ArrayInfo.BottomCellArray[i].cellID);
                            printf("!!!!!!\\n!!!!!!\\n %d \n",i);
                            i++;
                            left=right+1;
                            right=left;
                        }
                        else if( right-left+1==ArrayInfo.BottomCellArray[i].libCellSizeX && !isValidPlacement(PlacementState,row, left, right) ){
                            left=left+1;
                            right=right+1;
                        }
                        else{
                            right++;
                        }
                        if(( left >= currentDie->rowLength) || ( right >= currentDie->rowLength)){
                            row++;
                            left=currentDie->startX;
                            right=left;
                            break;
                        }

                    }
                }
            }
        }
        currentDie->PlacementState = PlacementState;
    }   
}

void printPlacementState(Die current_die){
    for(int i=0; i<(int)current_die.PlacementState.size(); i++){
        for(int j=0; j<(int)current_die.PlacementState[i].size(); j++){
            printf("%d ", current_die.PlacementState[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}