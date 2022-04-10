

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
        int MaxUtil;
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
    }Tech_menu;

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


    Die *createDie();

    Tech_menu *create_tech_menu();

    void create_libcell(Libcell *libcell, int libcell_count);

    void create_pinarray(Pin *pinarray, int pin_count);

    void create_InstanceArray(int NumInstances, InstanceNode *Instancearray);
    void create_netarray(int NumNets, Net *Netarray);

    void create_nodearray(int number_connection, Net *Netarray, int index);

    void read_one_blank_line(FILE *input);

    void readfile(FILE *input,Die *top_die,Die *bottom_die,hybrid_terminal *term,int *NumTechnologies, Tech_menu **tech_menu, int *NumInstances, InstanceNode *Instancearray, int *NumNets, Net *Netarray);


#endif