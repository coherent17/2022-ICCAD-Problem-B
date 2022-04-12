#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "readfile.h"

/*

void create_InstanceArray(int NumInstances, InstanceNode *Instancearray){
    Instancearray = (InstanceNode *)malloc(sizeof(InstanceNode) * NumInstances);
    for(int i = 0; i < NumInstances; i++){
        Instancearray[i].instance_name = (char *)malloc(sizeof(char) * 5);
        Instancearray[i].tech = (char *)malloc(sizeof(char) * 5);
        Instancearray[i].standard_cell_manufacture = (char *)malloc(sizeof(char) * 7);
    }
}

void create_netarray(int NumNets, Net *Netarray){
    Netarray = (Net *)malloc(sizeof(Net) * NumNets);
}

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
void printDieInfo(Die *top_die, Die *bottom_die){
    printf("\nTop Die Information:\n");
    printf("DieSize <lowerLeftX> <lowerLeftY> <upperRightX> <upperRightY>: %d %d %d %d\n", (top_die->lowerLeftX), (top_die->lowerLeftY), (top_die->upperRightX), (top_die->upperRightY));
    printf("TopDieMaxUtil: %d\n", top_die->MaxUtil);
    printf("TopDieRows <startX> <startY> <rowLength> <rowHeight> <repeatCount>:");
    printf("%d %d %d %d %d\n", (top_die->startX), (top_die->startY), (top_die->rowLength), (top_die->rowHeight), (top_die->repeatCount));
    printf("TopDieTech <TechName>: %s\n\n", top_die->tech);


    printf("\nBottom Die Information:\n");
    printf("DieSize <lowerLeftX> <lowerLeftY> <upperRightX> <upperRightY>: %d %d %d %d\n", (bottom_die->lowerLeftX), (bottom_die->lowerLeftY), (bottom_die->upperRightX), (bottom_die->upperRightY));
    printf("BottomDieMaxUtil: %d\n", bottom_die->MaxUtil);
    printf("BottomDieRows <startX> <startY> <rowLength> <rowHeight> <repeatCount>:");
    printf("%d %d %d %d %d\n", (bottom_die->startX), (bottom_die->startY), (bottom_die->rowLength),(bottom_die->rowHeight), (bottom_die->repeatCount));
    printf("BottomDieTech <TechName>: %s\n\n", bottom_die->tech);
}

void readHybridTerminalInfo(FILE *input, Hybrid_terminal *terminal){
    //if the pass in file pointer is point to NULL -> abort and raise the exception
    assert(input);

    //read terminal size & spacing
    fscanf(input, "%*s %d %d",&(terminal->sizeX),&(terminal->sizeY));
    fscanf(input, "%*s %d",&(terminal->spacing));
    read_one_blank_line(input);
}

void printHybridTerminalInfo(Hybrid_terminal *terminal){
    printf("\nHybrid Terminal Information:\n");
    printf("TerminalSize <sizeX> <sizeY>: %d %d\n", terminal->sizeX, terminal->sizeY);
    printf("TerminalSpacing <spacing>: %d\n\n", terminal->spacing);
}

void readTechnology(FILE *input, int *NumTechnologies, Tech_menu **tech_menu){
    //if the pass in file pointer is point to NULL -> abort and raise the exception
    assert(input);

    //read number of technologies
    fscanf(input, "%*s %d",&(*NumTechnologies));
    //allocate memory for the tech menu, might be 2 tech or 1 tech ex: TA TB (2) / TA TA (1) / TB TB (1)
    *tech_menu = (Tech_menu *)malloc(sizeof(Tech_menu) * (*NumTechnologies));

    for(int i = 0; i < *NumTechnologies; i++){
        fscanf(input, "%*s %s %d", (tech_menu[i])->tech, &(tech_menu[i]->libcell_count));
        printf("%d\n", (tech_menu[i])->libcell_count);
        (tech_menu[i])->libcell = (Libcell *)malloc(sizeof(Libcell) * (tech_menu[i])->libcell_count);

        //read libcell libcell count time
        for(int j = 0; j < tech_menu[i]->libcell_count; j++){
            fscanf(input, "%*s %s %d %d %d", tech_menu[i]->libcell[j].libCellName, &(tech_menu[i]->libcell[j].libCellSizeX), &(tech_menu[i]->libcell[j].libCellSizeY), &(tech_menu[i]->libcell[j].pinCount));
            tech_menu[i]->libcell[j].pinarray = (Pin *)malloc(sizeof(Pin) * tech_menu[i]->libcell[j].pinCount);

            //read pinarray pinarray_count time
            for(int k = 0; k < tech_menu[i]->libcell[j].pinCount; k++){
                fscanf(input, "%*s %s %d %d", tech_menu[i]->libcell[j].pinarray[k].pinName, &(tech_menu[i]->libcell[j].pinarray[k].pinLocationX), &(tech_menu[i]->libcell[j].pinarray[k].pinLocationY));
            }
        }
    }
    read_one_blank_line(input);
}

/*

void readfile(FILE *input,int *NumTechnologies, Tech_menu *tech_menu, int *NumInstances, InstanceNode *Instancearray, int *NumNets, Net *Netarray){
    assert(input);


    //read standard cell belong to which manufacture
    fscanf(input, "%*s %d", &(*NumInstances));
    create_InstanceArray(*NumInstances, Instancearray);
    for(int i = 0; i < *NumInstances; i++){
        fscanf(input, "%*s %s %s", Instancearray[i].instance_name, Instancearray[i].standard_cell_manufacture);
    }
    read_one_blank_line(input);

    //read the circuit netlist
    fscanf(input, "%*s %d", &(*NumNets));

    //allocate memory for Netarray
    create_netarray(*NumNets, Netarray);

    for(int i = 0; i < *NumNets; i++){
        fscanf(input, "%*s %s %d", Netarray[i].net_name , &Netarray[i].number_connection);

        //allocate memory for the detail connection in the net
        create_nodearray(Netarray[i].number_connection, Netarray, i);

        for(int j = 0; j < Netarray[i].number_connection; j++){
            char buffer[20];
            fscanf(input, "%*s %s", buffer);

            //divide the string by using delimiter "/"
            char *token = strtok(buffer, "/");
            strcpy(Netarray[i].NodeArray[j].belongCell, token);
            token = strtok(NULL, " ");
            strcpy(Netarray[i].NodeArray[j].Pin_number, token);
        }
    }
    fclose(input);
}

*/