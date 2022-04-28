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

void readTechnologyInfo(FILE *input, int *NumTechnologies, vector <Tech_menu> &TechMenu){
    assert(input);

    //read number of technologies
    fscanf(input, "%*s %d",&(*NumTechnologies));

    for(int i = 0; i < *NumTechnologies; i++){
        Tech_menu temp;
        fscanf(input, "%*s %s %d", temp.tech, &(temp.libcell_count));

        vector <Libcell> temp_libcell(temp.libcell_count);

        //read libcell libcell count time
        for(int j = 0; j < temp.libcell_count; j++){
            fscanf(input, "%*s %s %d %d %d", temp_libcell[j].libCellName, &(temp_libcell[j].libCellSizeX), &(temp_libcell[j].libCellSizeY), &(temp_libcell[j].pinCount));
            vector <Pin> temp_pinarray(temp_libcell[j].pinCount);

            //read pinarray pinarray_count time
            for(int k = 0; k < temp_libcell[j].pinCount; k++){
                fscanf(input, "%*s %s %d %d", temp_pinarray[k].pinName, &(temp_pinarray[k].pinLocationX), &(temp_pinarray[k].pinLocationY));
            }
            temp_libcell[j].pinarray = temp_pinarray;
        }
        temp.libcell = temp_libcell;
        TechMenu.emplace_back(temp);
    }
    read_one_blank_line(input);
}

void printTechnologyInfo(int NumTechnologies, vector <Tech_menu> TechMenu){
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

void readInstanceInfo(FILE *input, int *NumInstances, vector <Instance> &InstanceArray){
    assert(input);
    
    fscanf(input, "%*s %d", &(*NumInstances));

    for(int i = 0; i < *NumInstances; i++){
        Instance temp;
        fscanf(input, "%*s %s %s", temp.instName, temp.libCellName);
        InstanceArray.emplace_back(temp);
    }
    read_one_blank_line(input);
}

void printInstanceInfo(int NumInstances, vector <Instance> InstanceArray){
    printf("NumInstances <instanceCount>: %d\n", NumInstances);
    for(int i = 0; i < NumInstances; i++){
        printf("\tInst <instName> <libCellName>: %s %s\n", InstanceArray[i].instName, InstanceArray[i].libCellName);
    }
    printf("\n");
}

void readNetInfo(FILE *input, int *NumNets, vector <RawNet> &NetArray){
    assert(input);

    fscanf(input, "%*s %d", &(*NumNets));
    for(int i = 0; i < *NumNets; i++){
        RawNet temp;
        fscanf(input, "%*s %s %d", temp.netName , &temp.numPins);

        //allocate memory for the detail connection in the net
        vector <NetConnection> temp_connection(temp.numPins);

        for(int j = 0; j < temp.numPins; j++){
            char buffer[BUFFER_SIZE];
            fscanf(input, "%*s %s", buffer);
            //divide the string by using delimiter "/"
            char *token = strtok(buffer, "/");
            strcpy(temp_connection[j].instName, token);
            token = strtok(NULL, " ");
            strcpy(temp_connection[j].libPinName, token);
        }
        temp.Connection = temp_connection;
        NetArray.emplace_back(temp);
    }
    fclose(input);
}

void printNetInfo(int NumNets, vector <RawNet> NetArray){
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