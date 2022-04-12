#ifndef _READFILE_H_
#define _READFILE_H_

#define TECH_NAME_SIZE 10
#define PIN_NAME_SIZE 10
#define LIBCELL_NAME_SIZE 10

typedef struct _die{
    int lowerLeftX;
    int lowerLeftY;
    int upperRightX;
    int upperRightY;
    int size_x;
    int size_y;
    int MaxUtil;
    int startX;
    int startY;
    int rowLength;
    int rowHeight;
    int repeatCount;
    char tech[TECH_NAME_SIZE];
}Die;

typedef struct _terminal{
    int sizeX;
    int sizeY;
    int spacing;
}Hybrid_terminal;

typedef struct _pin{
    char pinName[PIN_NAME_SIZE]; //P1, P2, ...
    int pinLocationX;
    int pinLocationY;
}Pin;

typedef struct _Libcell{
    char libCellName[LIBCELL_NAME_SIZE]; //MC1,MC2...
    int libCellSizeX;
    int libCellSizeY;
    int pinCount;
    Pin *pinarray;
}Libcell;

typedef struct _tech_menu{
    char tech[TECH_NAME_SIZE]; //TA,TB
    int libcell_count;
    Libcell *libcell;
}Tech_menu;


void readDieInfo(FILE *input, Die *top_die, Die *bottom_die);
void printDieInfo(Die *top_die, Die *bottom_die);
void readHybridTerminalInfo(FILE *input, Hybrid_terminal *terminal);
void printHybridTerminalInfo(Hybrid_terminal *terminal);
void readTechnology(FILE *input, int *NumTechnologies, Tech_menu **tech_menu);

///////////////////////////////////////////

/*


typedef struct _InstanceNode{
    char *instance_name;  //C1, C2, ...
    char *tech;           //TA, TB, ...
    char *standard_cell_manufacture; //MC1, MC2, ...
}InstanceNode;

typedef struct _Node{
    char *belongCell;
    char *Pin_number;
}Node;

typedef struct _Net{
    char *net_name;
    int number_connection;
    Node *NodeArray;       //store number_connection of element in NodeArray
}Net;


*/

#endif