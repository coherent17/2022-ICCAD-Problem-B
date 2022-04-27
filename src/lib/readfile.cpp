#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "readfile.h"

#define BUFFER_SIZE 15

void read_one_blank_line(FILE *input){
    assert(input);
    fgetc(input);
}

void readDieInfo(FILE *input, Die *top_die, Die *bottom_die){
    assert(input);

    //read DieSize of the top die and the bottom size
    fscanf(input,"%*s %d %d %d %d", &(top_die->lowerLeftX), &(top_die->lowerLeftY), &(top_die->upperRightX), &(top_die->upperRightY));
    bottom_die->lowerLeftX = top_die->lowerLeftX;
    bottom_die->lowerLeftY = top_die->lowerLeftY;
    bottom_die->upperRightX = top_die->upperRightX;
    bottom_die->upperRightY = top_die->upperRightY;
    read_one_blank_line(input);

    //read maximum Utilization of the top die and the bottom die
    fscanf(input,"%*s %d", &(top_die->MaxUtil));
    fscanf(input,"%*s %d", &(bottom_die->MaxUtil));
    read_one_blank_line(input);

    //read DieRows Information
    fscanf(input,"%*s %d %d %d %d %d",&(top_die->startX), &(top_die->startY), &(top_die->rowLength),&(top_die->rowHeight), &(top_die->repeatCount));
    fscanf(input,"%*s %d %d %d %d %d",&(bottom_die->startX), &(bottom_die->startY), &(bottom_die->rowLength),&(bottom_die->rowHeight), &(bottom_die->repeatCount));
    read_one_blank_line(input);

    //read DieTech
    fscanf(input,"%*s %s",top_die->tech);
    fscanf(input,"%*s %s",bottom_die->tech);
    read_one_blank_line(input);
}

//print the detail die information
void printDieInfo(Die top_die, Die bottom_die){
    printf("\nTop Die Information:\n");
    printf("DieSize <lowerLeftX> <lowerLeftY> <upperRightX> <upperRightY>: %d %d %d %d\n", top_die.lowerLeftX, top_die.lowerLeftY, top_die.upperRightX, top_die.upperRightY);
    printf("TopDieMaxUtil: %d\n", top_die.MaxUtil);
    printf("TopDieRows <startX> <startY> <rowLength> <rowHeight> <repeatCount>:");
    printf("%d %d %d %d %d\n", top_die.startX, top_die.startY, top_die.rowLength, top_die.rowHeight, top_die.repeatCount);
    printf("TopDieTech <TechName>: %s\n\n", top_die.tech);


    printf("\nBottom Die Information:\n");
    printf("DieSize <lowerLeftX> <lowerLeftY> <upperRightX> <upperRightY>: %d %d %d %d\n", bottom_die.lowerLeftX, bottom_die.lowerLeftY, bottom_die.upperRightX, bottom_die.upperRightY);
    printf("BottomDieMaxUtil: %d\n", bottom_die.MaxUtil);
    printf("BottomDieRows <startX> <startY> <rowLength> <rowHeight> <repeatCount>:");
    printf("%d %d %d %d %d\n", bottom_die.startX, bottom_die.startY, bottom_die.rowLength, bottom_die.rowHeight, bottom_die.repeatCount);
    printf("BottomDieTech <TechName>: %s\n\n", bottom_die.tech);
}

void readHybridTerminalInfo(FILE *input, Hybrid_terminal *terminal){
    assert(input);

    //read terminal size & spacing
    fscanf(input, "%*s %d %d",&(terminal->sizeX),&(terminal->sizeY));
    fscanf(input, "%*s %d",&(terminal->spacing));
    read_one_blank_line(input);
}

void printHybridTerminalInfo(Hybrid_terminal terminal){
    printf("\nHybrid Terminal Information:\n");
    printf("TerminalSize <sizeX> <sizeY>: %d %d\n", terminal.sizeX, terminal.sizeY);
    printf("TerminalSpacing <spacing>: %d\n\n", terminal.spacing);
}

void readTechnologyInfo(FILE *input, int *NumTechnologies, Tech_menu **TechMenu){
    assert(input);

    //read number of technologies
    fscanf(input, "%*s %d",&(*NumTechnologies));
    //allocate memory for the tech menu, might be 2 tech or 1 tech ex: TA TB (2) / TA TA (1) / TB TB (1)
    *TechMenu = (Tech_menu *)malloc(sizeof(Tech_menu) * (*NumTechnologies));

    for(int i = 0; i < *NumTechnologies; i++){
        fscanf(input, "%*s %s %d", ((*TechMenu)[i]).tech, &((*TechMenu)[i].libcell_count));
        ((*TechMenu)[i]).libcell = (Libcell *)malloc(sizeof(Libcell) * ((*TechMenu)[i]).libcell_count);

        //read libcell libcell count time
        for(int j = 0; j < (*TechMenu)[i].libcell_count; j++){
            fscanf(input, "%*s %s %d %d %d", (*TechMenu)[i].libcell[j].libCellName, &((*TechMenu)[i].libcell[j].libCellSizeX), &((*TechMenu)[i].libcell[j].libCellSizeY), &((*TechMenu)[i].libcell[j].pinCount));
            ((*TechMenu)[i]).libcell[j].pinarray = (Pin *)malloc(sizeof(Pin) * ((*TechMenu)[i]).libcell[j].pinCount);

            //read pinarray pinarray_count time
            for(int k = 0; k < (*TechMenu)[i].libcell[j].pinCount; k++){
                fscanf(input, "%*s %s %d %d", (*TechMenu)[i].libcell[j].pinarray[k].pinName, &((*TechMenu)[i].libcell[j].pinarray[k].pinLocationX), &((*TechMenu)[i].libcell[j].pinarray[k].pinLocationY));
            }
        }
    }
    read_one_blank_line(input);
}

void freeTech_menu(int NumTechnologies, Tech_menu **TechMenu){
    for(int i = 0; i < NumTechnologies; i++){
        for(int j = 0; j < (*TechMenu)[i].libcell_count; j++){
            free((*TechMenu)[i].libcell[j].pinarray);
        }
        free((*TechMenu)[i].libcell);
    }
    free(*TechMenu);
}

void printTechnologyInfo(int NumTechnologies, Tech_menu *TechMenu){
    printf("\nNumTechnologies <technologyCount>: %d\n\n", NumTechnologies);
    for(int i = 0; i < NumTechnologies; i++){
        printf("Tech <techName> <libCellCount>: %s %d:\n", TechMenu[i].tech, TechMenu[i].libcell_count);
        for(int j = 0; j < TechMenu[i].libcell_count; j++){
            printf("\tLibCell <libCellName> <libCellSizeX> <libCellSizeY> <pinCount>: %s %d %d %d\n", TechMenu[i].libcell[j].libCellName, TechMenu[i].libcell[j].libCellSizeX, TechMenu[i].libcell[j].libCellSizeY, TechMenu[i].libcell[j].pinCount);
            for(int k = 0; k < TechMenu[i].libcell[j].pinCount; k++){
                printf("\t\tPin <pinName> <pinLocationX> <pinLocationY>: %s %d %d\n", TechMenu[i].libcell[j].pinarray[k].pinName, TechMenu[i].libcell[j].pinarray[k].pinLocationX, TechMenu[i].libcell[j].pinarray[k].pinLocationY);
            }
            printf("\n");
        }
        printf("\n\n");
    }
    printf("\n");
}

void readInstanceInfo(FILE *input, int *NumInstances, Instance **InstanceArray){
    assert(input);
    
    fscanf(input, "%*s %d", &(*NumInstances));
    //allocate memory for InstanceArray:
    *InstanceArray = (Instance *)malloc(sizeof(Instance) * (*NumInstances));
    for(int i = 0; i < *NumInstances; i++){
        fscanf(input, "%*s %s %s", (*InstanceArray)[i].instName, (*InstanceArray)[i].libCellName);
    }
    read_one_blank_line(input);
}

void freeInstanceArray(int NumInstances, Instance **InstanceArray){
    free(*InstanceArray);
}

void printInstanceInfo(int NumInstances, Instance *InstanceArray){
    printf("NumInstances <instanceCount>: %d\n", NumInstances);
    for(int i = 0; i < NumInstances; i++){
        printf("\tInst <instName> <libCellName>: %s %s\n", InstanceArray[i].instName, InstanceArray[i].libCellName);
    }
    printf("\n");
}

void readNetInfo(FILE *input, int *NumNets, Net **NetArray){
    assert(input);

    fscanf(input, "%*s %d", &(*NumNets));
    //allocate memory for NetArray:
    *NetArray = (Net *)malloc(sizeof(Net) * (*NumNets));

    for(int i = 0; i < *NumNets; i++){
        fscanf(input, "%*s %s %d", (*NetArray)[i].netName , &(*NetArray)[i].numPins);

        //allocate memory for the detail connection in the net
        (*NetArray)[i].Connection = (NetConnection *)malloc(sizeof(NetConnection) * (*NetArray)[i].numPins);

        for(int j = 0; j < (*NetArray)[i].numPins; j++){
            char buffer[BUFFER_SIZE];
            fscanf(input, "%*s %s", buffer);
            //divide the string by using delimiter "/"
            char *token = strtok(buffer, "/");
            strcpy((*NetArray)[i].Connection[j].instName, token);
            token = strtok(NULL, " ");
            strcpy((*NetArray)[i].Connection[j].libPinName, token);
        }
    }
}

void freeNetArray(int NumNets, Net **NetArray){
    for(int i = 0; i < NumNets; i++){
        free((*NetArray)[i].Connection);
    }
    free(*NetArray); 
}

void printNetInfo(int NumNets, Net *NetArray){
    printf("\nNumNets <netCount>: %d\n", NumNets);
    for(int i = 0; i < NumNets; i++){
        printf("\tNet <netName> <numPins>: %s %d\n", NetArray[i].netName, NetArray[i].numPins);
        for(int j = 0; j < NetArray[i].numPins; j++){
            printf("\t\tPin <instName>/<libPinName>: %s / %s\n", NetArray[i].Connection[j].instName, NetArray[i].Connection[j].libPinName);
        }
        printf("\n");
    }
    printf("\n");
}

void realAllInfo(FILE *input, int *NumTechnologies, Tech_menu **TechMenu, Die *top_die, Die *bottom_die, Hybrid_terminal *terminal, int *NumInstances, Instance **InstanceArray, int *NumNets, Net **NetArray){
    readTechnologyInfo(input, NumTechnologies, TechMenu);
    readDieInfo(input, top_die, bottom_die);
    readHybridTerminalInfo(input, terminal);
    readInstanceInfo(input, NumInstances, InstanceArray);
    readNetInfo(input, NumNets, NetArray);
}