#ifndef _READFILE_H_
#define _READFILE_H_

#define TECH_NAME_SIZE 10
#define PIN_NAME_SIZE 10
#define LIBCELL_NAME_SIZE 10
#define INSTANCE_NAME_SIZE 10
#define NET_NAME_SIZE 10

typedef struct _die{
    int lowerLeftX;
    int lowerLeftY;
    int upperRightX;
    int upperRightY;
    int size_x;
    int size_y;
    int MaxUtil;
    int startX;                 //start x of row
    int startY;                 //start y of row
    int rowLength;
    int rowHeight;
    int repeatCount;
    char tech[TECH_NAME_SIZE];

}Die;

typedef struct _terminal{
    int sizeX;
    int sizeY;
    int spacing;   //between terminals and between terminal and boundary
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

typedef struct _Instance{
    char tech[TECH_NAME_SIZE];              //TA, TB, ...
    char instName[INSTANCE_NAME_SIZE];      //C1, C2, ...
    char libCellName[LIBCELL_NAME_SIZE];    //MC1, MC2, ...
    int whichDie;                          //(0, 1) = (bottom die, top die)
}Instance;

typedef struct _NetConnection{
    char instName[INSTANCE_NAME_SIZE];      //C1, C2, ...
    char libPinName[PIN_NAME_SIZE];         //P1, P2, ...
}NetConnection;

typedef struct _Net{
    char netName[NET_NAME_SIZE];
    int numPins;
    NetConnection *Connection;
}Net;




void printDieInfo(Die top_die, Die bottom_die);
void printHybridTerminalInfo(Hybrid_terminal terminal);
void printTechnologyInfo(int NumTechnologies, Tech_menu *TechMenu);
void printInstanceInfo(int NumInstances, Instance *InstanceArray);
void printNetInfo(int NumNets, Net *NetArray);

void freeTech_menu(int NumTechnologies, Tech_menu **TechMenu);
void freeInstanceArray(int NumInstances, Instance **InstanceArray);
void freeNetArray(int NumNets, Net **NetArray);

void realAllInfo(FILE *input, int *NumTechnologies, Tech_menu **, Die *top_die, Die *bottom_die, Hybrid_terminal *terminal, int *NumInstances, Instance **InstanceArray, int *NumNets, Net **NetArray);

#endif