# 2022 ICCAD Problem B 
Problem B: 3D Placement with D2D Vertical Connections

> Author Chi-En Ho Chieh-En Wang

2022 ICCAD Problem B website: 
http://iccad-contest.org/Problems.html

Synchronize this note and source code in github:
https://github.com/coherent17/2022-ICCAD-Problem-B

[TOC]

## Introduction:

Spilt a large die into two or more small dies and increase the yield, and better cost! In this Problem, we are going to partition a large die into 2 dies with the same sizes. Try to find the placement of the standard cell and the hybrid bonding so that minimize the Half-Perimeter Wirelength(HPWL)

![](https://i.imgur.com/DzbNdss.png)

## Goal:
Minimum the HPWL of the 3D placement

## Constraints:
*    The total number of hybrid bonding terminals is limited. 
*    All the hybrid bonding terminals need to be placed on the top-most layer of the die with required terminal size and ***terminal spacing requirement***.
*    Each inter-die connections need to have 1 and only 1 hybrid bonding terminal.
*    The technology process of the 2 dies will be given.
*    The cell size, cell height, cell pin location of the same logical library cell would be different in each technology. 
*    The center point of the hybrid bonding terminals needs to be included in the HPWL calculation for each die.
*    All the cells need to be placed on row and cannot have overlap with each other. 
*    Max placement utilization constraint must be satisfied.
*    The minimum resolution of all the coordinate values is integer.


## Procedure
1. Partition the big dies into 2 dies (maybe use shemetis?)
2. Place and legalize both the top die and bottom die. 
3. Place the Hydrogen Bond so that minimize the HPWL

## Input Format

### 1. Netlist 
#### *Syntax*
```Syntax=
NumInstances <instanceCount> 
Inst <instName> <libCellName> 
NumNets <netCount> 
Net <netName> <numPins> 
Pin <instName>/<libPinName> 
```
#### *Example*
```syntax=
NumInstances 2 
Inst C1 MC1 
Inst C2 MC3 
NumNets 1 
Net N1 2 
Pin C1/P2 
Pin C2/P1 
```

### 2. Die size for both bottom die and top die (would be the same)
#### *Syntax*
```Syntax=
DieSize <lowerLeftX> <lowerLeftY> <upperRightX> <upperRightY> 
```
#### *Example*
```Syntax=
DieSize 0 0 500 450 
```
### 3. Max placement utilization ratio for each die
![](https://i.imgur.com/52UWNh7.png)

#### *Syntax*
```Syntax=
TopDieMaxUtil <util> 
BottomDieMaxUtil <util> 
```
#### *Example*
```Syntax=
TopDieMaxUtil 75 
BottomDieMaxUtil 80 
```
### 4. Placement rows of each die  
* The given rows would start from (0, 0) and cover the entire die. 
#### *Syntax*
```Syntax=
TopDieRows <startX> <startY> <rowLength> <rowHeight> <repeatCount> 
BottomDieRows <startX> <startY> <rowLength> <rowHeight> <repeatCount> 
```
#### *Example*
```Syntax=
TopDieRows 0 0 500 10 45 
BottomDieRows 0 0 500 15 30 
```
### 5. Std cell library for each die. 
* If the 2 dies are with different technologies, the given std cell library for each die would have equivalent logic 
#### *Syntax*
```Syntax=
NumTechnologies <technologyCount> 
Tech <techName> <libCellCount> 
LibCell <libCellName> <libCellSizeX> <libCellSizeY> <pinCount> 
Pin <pinName> <pinLocationX> <pinLocationY> 
```
#### *Example*
```Syntax=
NumTechnologies 2 
Tech TA 2 
LibCell MC1 5 10 1 
Pin P1 2 7 
LibCell MC2 7 10 2 
Pin P1 5 3 
Pin P2 3 6 
Tech TB 2 
LibCell MC1 5 15 1 
Pin P1 2 11 
LibCell MC2 7 15 2 
Pin P1 2 12 
Pin P2 3 3 
```
### 6. Hybrid bonding terminal size & the required spacing
* Hybrid bonding terminal size & the required spacing between 2 terminals and between terminal and die boundary
#### *Syntax*
```Syntax=
TerminalSize <sizeX> <sizeY> 
TerminalSpacing <spacing> 
```
#### *Example*
```Syntax=
TerminalSize 20 20 
TerminalSpacing 15 
```
## Output Format

### 1. Top die placement result

#### *Syntax*
```Syntax=
TopDiePlacement <InstCount> 
Inst <instName> <locationX> <locationY> 
```

#### *Example*
```Syntax=
TopDiePlacement 2 
Inst C1 0 10
Inst C4 15 20 
```
### 2. Bottom die placement result

#### *Syntax*
```Syntax=
BottomDiePlacement <InstCount> 
Inst <instName> <locationX> <locationY> 
```

#### *Example*
```Syntax=
BottomDiePlacement 3 
Inst C2 20 15 
Inst C3 23 30 
Inst C5 50 15 
```
### 3. Hybrid bonding terminal placement result with net information 

#### *Syntax*
```Syntax=
    NumTerminals <TerminalCount> 
    Terminal <netName> <locationX> <locationY> 
```

#### *Example*
```Syntax=
    NumTerminals 2 
    Terminal N1 100 200 
    Terminal N3 180 180 
```
* Use the same coordinate system for each die, of which bottom-left points are both $(0,0)$.
* The outputted coordinate is lower-left coordinate of the cell.
* All the cells can NOT be flipped nor mirrored.

## Evaluation


## To Do List
- [ ] Learn Cuda or [Pthread](https://github.com/coherent17/C-fun-things/tree/main/pthread) or [Openmp](https://hackmd.io/OGVqHeg0RSW1ObVKPGTGOg) to parallel programming
- [ ] Parse the input testcase with [format](https://hackmd.io/JZg4rwd7TS-oXYYv2mjYKA?view#Input-Format)
- [ ] Write a global timer. (Runtime limit: 1hr)
- [ ] 2-way partition (shemetis?)
- [ ] Placement the Standard Cell by simulate annealing
    - [ ] Stage 1
        *    Standard Cell are moved between different rows and in the same row.
        *    Standard Cell overlaps are allowed.
        *    When the temperature is reached below a certain value,stage 2 begins.
    - [ ] Stage 2
        *    Remove overlaps.
        *    Annealing process continues, but only interchanges adjacent standard cells within the same row.
- [ ] Placement the Hybrid Bonding by calculating the bounding box.
- [ ] Calculate $HPWL_{top\ die} + HPWL_{bottom\ die}$
- [ ] Replacement if needed
- [ ] Output the result with [format](https://hackmd.io/JZg4rwd7TS-oXYYv2mjYKA?view#Output-Format)