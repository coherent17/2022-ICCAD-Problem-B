#include <stdlib.h>
#include <stdio.h>
#include "readfile.h"

Die *createDie(){
    Die *newDie = malloc(sizeof(Die));
    newDie->tech = malloc(sizeof(char)*3);
}

tech_menu *create_tech_menu(){
    tech_menu *new_tech_menu = malloc(sizeof(tech_menu));
    tech_menu->tech = malloc(sizeof(char)*3);
}

void create_libcell(Libcell *libcell, int libcell_count){
    libcell = malloc(sizeof(Libcell)*libcell_count);
    for(int i=0 ; i < libcell_count ; i++){
        libcell[i]->libcell_name = malloc(sizeof(char)*5);
    }
}

void read_one_blank_line(FILE *input){
    fscanf(input,"%*s");
}

void read_die_information(char *input_name,Die *top_die,Die *bottom_die,hybrid_terminal *term,int *NumTechnologies){
    FILE *input=fopen(input_name,"r");
    
    //read DieSize
    fscanf(input,"%*s %*d %*d %d %d",&(top_die->size_x),&(top_die->size_y));
    bottom_die->size_x = top_die->size_x;
    bottom_die->size_x = top_die->size_x;
    read_one_blank_line(input);

    //read Utilization
    fscanf(input,"%*s %d", &(top_die->MaxUtil));
    top_die->MaxUtil = top_die->MaxUtil /100 ;
    fscanf(input,"%*s %d", &(bottom_die->MaxUtil));
    bottom_die->MaxUtil = bottom_die->MaxUtil /100 ;
    read_one_blank_line(input);

    //read DieRows
    fscanf(input,"%*s %*d %*d %d %d %d",&(top_die->size_x),&(top_die->row_height),&(top_die->row_number));
    fscanf(input,"%*s %*d %*d %d %d %d",&(bottom_die->size_x),&(bottom_die->row_height),&(bottom_die->row_number));
    read_one_blank_line(input);

    //read DieTech
    fscanf(input,"%*s %s",&(top_die->tech));
    fscanf(input,"%*s %s",&(bottom_die->tech));
    read_one_blank_line(input);

    //read terminal size & spacing
    fscanf("%*s %d %d",&(term->size_x),&(term->size_y));
    fscanf("%*s %d",&(term->spacing));
    read_one_blank_line(input);

    //read number of technologies
    fscanf("%*s %d",&(*NumTechnologies));

    //read different tech

    //allocate memory for the tech menu
    tech_menu **tech_menu = malloc(sizeof(tech_menu *) * (*NumTechnologies));
    for(int i=0 ; i < *NumTechnologies ; i++){
        tech_menu[i]= create_tech_menu();
    }

    for(int i = 0; i < *NumTechnologies; i++){
        fscanf(input, "%*s %s %d", &(tech_menu[i]->tech), &(tech_menu[i]->libcell_count));
        create_libcell(tech_menu[i]->libcell, tech_menu[i]->libcell_count);

        //read libcell libcell count time
        for(int j = 0; j < tech_menu[i]->libcell_count; j++){
            Libcell *curcell = tech_menu[i]->libcell;
            fscanf(input, "%*s %s %d %d %d", curcell[j]->);
        }
    }



}




