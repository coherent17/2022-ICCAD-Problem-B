#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "readfile.h"

#define BUFFER_SIZE 15

/*


void create_nodearray(int number_connection, Net *Netarray, int index){
    Netarray[index].NodeArray = (Node *)malloc(sizeof(Node) * number_connection);
}
*/

void read_one_blank_line(FILE *input){
    fgetc(input);
}


void readDieInfo(FILE *input, Die *top_die, Die *bottom_die){
    //if the pass in file pointer is point to NULL -> abort and raise the exception
    assert(input);

    //read DieSize of the top die and the bottom size
    //***********************************************************************
    fscanf(input,"%*s %d %d %d %d", &(top_die->lowerLeftX), &(top_die->lowerLeftY), &(top_die->upperRightX), &(top_die->upperRightY));
    bottom_die->lowerLeftX = top_die->lowerLeftX;
    bottom_die->lowerLeftY = top_die->lowerLeftY;
    bottom_die->upperRightX = top_die->upperRightX;
    bottom_die->upperRightY = top_die->upperRightY;
    read_one_blank_line(input);
    //***********************************************************************



    //read maximum Utilization of the top die and the bottom die
    //***********************************************************************
    fscanf(input,"%*s %d", &(top_die->MaxUtil));
    fscanf(input,"%*s %d", &(bottom_die->MaxUtil));
    read_one_blank_line(input);
    //***********************************************************************



    //read DieRows Information
    //************************************************************************************************************
    fscanf(input,"%*s %d %d %d %d %d",&(top_die->startX), &(top_die->startY), &(top_die->rowLength),&(top_die->rowHeight), &(top_die->repeatCount));
    fscanf(input,"%*s %d %d %d %d %d",&(bottom_die->startX), &(bottom_die->startY), &(bottom_die->rowLength),&(bottom_die->rowHeight), &(bottom_die->repeatCount));
    read_one_blank_line(input);
    //************************************************************************************************************



    //read DieTech
    //***********************************************************************
    fscanf(input,"%*s %s",top_die->tech);
    fscanf(input,"%*s %s",bottom_die->tech);
    read_one_blank_line(input);
    //***********************************************************************
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
    //if the pass in file pointer is point to NULL -> abort and raise the exception
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

void readTechnologyInfo(FILE *input, int *NumTechnologies, Tech_menu **tech_menu){
    //if the pass in file pointer is point to NULL -> abort and raise the exception
    assert(input);

    //read number of technologies
    fscanf(input, "%*s %d",&(*NumTechnologies));
    //allocate memory for the tech menu, might be 2 tech or 1 tech ex: TA TB (2) / TA TA (1) / TB TB (1)
    *tech_menu = (Tech_menu *)malloc(sizeof(Tech_menu) * (*NumTechnologies));

    for(int i = 0; i < *NumTechnologies; i++){
        fscanf(input, "%*s %s %d", ((*tech_menu)[i]).tech, &((*tech_menu)[i].libcell_count));
        ((*tech_menu)[i]).libcell = (Libcell *)malloc(sizeof(Libcell) * ((*tech_menu)[i]).libcell_count);

        //read libcell libcell count time
        for(int j = 0; j < (*tech_menu)[i].libcell_count; j++){
            fscanf(input, "%*s %s %d %d %d", (*tech_menu)[i].libcell[j].libCellName, &((*tech_menu)[i].libcell[j].libCellSizeX), &((*tech_menu)[i].libcell[j].libCellSizeY), &((*tech_menu)[i].libcell[j].pinCount));
            ((*tech_menu)[i]).libcell[j].pinarray = (Pin *)malloc(sizeof(Pin) * ((*tech_menu)[i]).libcell[j].pinCount);

            //read pinarray pinarray_count time
            for(int k = 0; k < (*tech_menu)[i].libcell[j].pinCount; k++){
                fscanf(input, "%*s %s %d %d", (*tech_menu)[i].libcell[j].pinarray[k].pinName, &((*tech_menu)[i].libcell[j].pinarray[k].pinLocationX), &((*tech_menu)[i].libcell[j].pinarray[k].pinLocationY));
            }
        }
    }
    read_one_blank_line(input);
}

void printTechnologyInfo(int NumTechnologies, Tech_menu *tech_menu){
    printf("\nNumTechnologies <technologyCount>: %d\n\n", NumTechnologies);
    for(int i = 0; i < NumTechnologies; i++){
        printf("Tech <techName> <libCellCount>: %s %d:\n", tech_menu[i].tech, tech_menu[i].libcell_count);
        for(int j = 0; j < tech_menu[i].libcell_count; j++){
            printf("\tLibCell <libCellName> <libCellSizeX> <libCellSizeY> <pinCount>: %s %d %d %d\n", tech_menu[i].libcell[j].libCellName, tech_menu[i].libcell[j].libCellSizeX, tech_menu[i].libcell[j].libCellSizeY, tech_menu[i].libcell[j].pinCount);
            for(int k = 0; k < tech_menu[i].libcell[j].pinCount; k++){
                printf("\t\tPin <pinName> <pinLocationX> <pinLocationY>: %s %d %d\n", tech_menu[i].libcell[j].pinarray[k].pinName, tech_menu[i].libcell[j].pinarray[k].pinLocationX, tech_menu[i].libcell[j].pinarray[k].pinLocationY);
            }
            printf("\n");
        }
        printf("\n\n");
    }
    printf("\n");
}

void readInstanceInfo(FILE *input, int *NumInstances, Instance **InstanceArray){
    //if the pass in file pointer is point to NULL -> abort and raise the exception
    assert(input);
    
    fscanf(input, "%*s %d", &(*NumInstances));
    //allocate memory for InstanceArray:
    *InstanceArray = (Instance *)malloc(sizeof(Instance) * (*NumInstances));
    for(int i = 0; i < *NumInstances; i++){
        fscanf(input, "%*s %s %s", (*InstanceArray)[i].instName, (*InstanceArray)[i].libCellName);
    }
    read_one_blank_line(input);
}

void printInstanceInfo(int NumInstances, Instance *InstanceArray){
    printf("NumInstances <instanceCount>: %d\n", NumInstances);
    for(int i = 0; i < NumInstances; i++){
        printf("\tInst <instName> <libCellName>: %s %s\n", InstanceArray[i].instName, InstanceArray[i].libCellName);
    }
    printf("\n");
}

void readNetInfo(FILE *input, int *NumNets, Net **NetArray){
    //if the pass in file pointer is point to NULL -> abort and raise the exception
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