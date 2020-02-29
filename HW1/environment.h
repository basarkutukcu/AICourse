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
    int clusterNum;
}GridCell;

void createGridWorld(int n);
int getClusterNum(int x, int y);
void setClusterNum(int x, int y, int cNum);
int getGoldNum(int x, int y);
void setGoldNum(int x, int y, int val);
int getIsRobotCarryGold(int x, int y);
void setIsRobotCarryGold(int x, int y, int val);
int getOccupation(int x, int y);
void setOccupation(int x, int y, int val);
void printCurrGrid();