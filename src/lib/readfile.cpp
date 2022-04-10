#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "readfile.h"

Die *createDie(){
    Die *newDie = (Die *)malloc(sizeof(Die));
    newDie->tech = (char *)malloc(sizeof(char)*3);
    return newDie;
}

Tech_menu *create_tech_menu(){
    Tech_menu *new_tech_menu = (Tech_menu *)malloc(sizeof(Tech_menu));
    new_tech_menu->tech = (char *)malloc(sizeof(char)*3);
    return new_tech_menu;
}

void create_libcell(Libcell *libcell, int libcell_count){
    libcell = (Libcell *)malloc(sizeof(Libcell)*libcell_count);
    for(int i=0 ; i < libcell_count ; i++){
        libcell[i].libcell_name = (char *)malloc(sizeof(char)*5);
    }
}

void create_pinarray(Pin *pinarray, int pin_count){
    pinarray = (Pin *)malloc(sizeof(Pin) * pin_count);
    for(int i = 0; i < pin_count; i++){
        pinarray[i].pin_name = (char *)malloc(sizeof(char)*5);
    }
}

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

void read_one_blank_line(FILE *input){
    fscanf(input,"%*s");
}

void readfile(FILE *input,Die *top_die,Die *bottom_die,hybrid_terminal *term,int *NumTechnologies, Tech_menu **tech_menu, int *NumInstances, InstanceNode *Instancearray, int *NumNets, Net *Netarray){
    
    //read DieSize
    fscanf(input,"%*s %*d %*d %d %d",&(top_die->size_x),&(top_die->size_y));
    bottom_die->size_x = top_die->size_x;
    bottom_die->size_x = top_die->size_x;
    read_one_blank_line(input);

    //read Utilization
    fscanf(input,"%*s %d", &(top_die->MaxUtil));
    fscanf(input,"%*s %d", &(bottom_die->MaxUtil));
    read_one_blank_line(input);

    //read DieRows
    fscanf(input,"%*s %*d %*d %d %d %d",&(top_die->size_x),&(top_die->row_height),&(top_die->row_number));
    fscanf(input,"%*s %*d %*d %d %d %d",&(bottom_die->size_x),&(bottom_die->row_height),&(bottom_die->row_number));
    read_one_blank_line(input);

    //read DieTech
    fscanf(input,"%*s %s",top_die->tech);
    fscanf(input,"%*s %s",bottom_die->tech);
    read_one_blank_line(input);

    //read terminal size & spacing
    fscanf(input, "%*s %d %d",&(term->size_x),&(term->size_y));
    fscanf(input, "%*s %d",&(term->spacing));
    read_one_blank_line(input);

    //read number of technologies
    fscanf(input, "%*s %d",&(*NumTechnologies));

    //read different tech

    //allocate memory for the tech menu
    tech_menu = (Tech_menu **)malloc(sizeof(Tech_menu *) * (*NumTechnologies));
    for(int i=0 ; i < *NumTechnologies ; i++){
        tech_menu[i]= create_tech_menu();
    }

    for(int i = 0; i < *NumTechnologies; i++){
        fscanf(input, "%*s %s %d", tech_menu[i]->tech, &(tech_menu[i]->libcell_count));
        create_libcell(tech_menu[i]->libcell, tech_menu[i]->libcell_count);

        //read libcell libcell count time
        for(int j = 0; j < tech_menu[i]->libcell_count; j++){
            Libcell *curlibcell = &(tech_menu[i]->libcell[j]);
            fscanf(input, "%*s %s %d %d %d", curlibcell->libcell_name, &curlibcell->size_x, &curlibcell->size_y, &curlibcell->pin_count);
            create_pinarray(curlibcell->pinarray, curlibcell->pin_count);

            //read pinarray pinarray_count time
            for(int k = 0; k < curlibcell->pin_count; k++){
                Pin *curPin = &(curlibcell->pinarray[k]);
                fscanf(input, "%*s %s %d %d", curPin->pin_name, &curPin->x, &curPin->y);
            }
        }
    }
    read_one_blank_line(input);

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




