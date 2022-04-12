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

typedef struct _Instance{
    char tech[TECH_NAME_SIZE];              //TA, TB, ...
    char instName[INSTANCE_NAME_SIZE];      //C1, C2, ...
    char libCellName[LIBCELL_NAME_SIZE];    //MC1, MC2, ...
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


void readDieInfo(FILE *input, Die *top_die, Die *bottom_die);
void printDieInfo(Die top_die, Die bottom_die);
void readHybridTerminalInfo(FILE *input, Hybrid_terminal *terminal);
void printHybridTerminalInfo(Hybrid_terminal terminal);
void readTechnologyInfo(FILE *input, int *NumTechnologies, Tech_menu **tech_menu);
void printTechnologyInfo(int NumTechnologies, Tech_menu *tech_menu);
void readInstanceInfo(FILE *input, int *NumInstances, Instance **InstanceArray);
void printInstanceInfo(int NumInstances, Instance *InstanceArray);
void readNetInfo(FILE *input, int *NumNets, Net **NetArray);
void printNetInfo(int NumNets, Net *NetArray);

#endif