#ifndef _READFILE_H_
#define _READFILE_H_

    typedef struct _cor{ //record coordinate
        int x;
        int y;
    }coordinate;

    typedef struct _standard_cell{
        int number;
        int size_x;
        int size_y;
        int num_of_pin;
        coordinate *pinarray;
    }cell;

    typedef struct _die_information{
        int size_x;
        int size_y;
        int row_height;
        int row_number;
        double MaxUtil;
        char *tech;
    }Die;

    typedef struct _terminal{
        int size_x;
        int size_y;
        int spacing;
    }hybrid_terminal;

    typedef struct _pin{
        char *pin_name; //P1, P2, ...
        int x;
        int y;
    }Pin;

    typedef struct _Libcell{
        char *libcell_name; //MC1,MC2...
        int size_x;
        int size_y;
        int pin_count;
        Pin *pinarray;
    }Libcell;

    typedef struct _tech_menu{
        char *tech; //TA,TB
        int libcell_count;
        Libcell *libcell; 
    }tech_menu;

    typedef struct _InstanceNode{
        char *instance_name;  //C1, C2, ...
        char *tech;           //TA, TB, ...
        char *standard_cell_manufacture; //MC1, MC2, ...
    }InstanceNode;

    typedef struct _Net{

    }

    typedef struct _Net{
        
    }Net;

#endif