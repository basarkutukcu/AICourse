#ifndef ENVIRONMENT_H 
#define ENVIRONMENT_H 

#define OCCUPATION_NONE     0
#define OCCUPATION_DEPOT    1
#define OCCUPATION_GOLD     2
#define OCCUPATION_OBSTACLE 3
#define OCCUPATION_ROBOT    4

typedef struct GridCells
{
    int occupation;    // Can be 'D', 'G', 'O', 'R', 'r', or 'x' (empty)
    int goldNum;    // valid if occupation == 'D' or 'G'
    int isRobotCarryGold;   // valid if occupation == 'r'
    int robotID;     // valid if occupation == 'r'
    float distanceToDepot;    // Simulating depot signal in the book
    int crumbNum;
}GridCell;

void    createGridWorld(int n);
int     getGoldNum(int x, int y);
void    setGoldNum(int x, int y, int val);
int     getIsRobotCarryGold(int x, int y);
void    setIsRobotCarryGold(int x, int y, int val);
int     getOccupation(int x, int y);
void    setOccupation(int x, int y, int val);
void    printCurrGrid();
int     getRobotID(int x, int y);
void    setRobotID(int x, int y, int val);
int     getTerminateSignal();
void    addToTotalGolds(int val);
void    calculateSignalStr();
int     getGridLen();
float   getSignalStr(int x, int y);
int     getcrumbNum(int x, int y);
void    setcrumbNum(int x, int y, int val);

#endif