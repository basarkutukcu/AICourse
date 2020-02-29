#ifndef ROBOT_H 
#define ROBOT_H 

typedef struct Robots
{
    int robotID;
    int x;
    int y;
    int isCarryGold;
}Robot;

void spawnRobots(int n);
void putRobotOnMap(int x, int y, int id);
void setTotalGolds(int totGold);
int getTotalGolds();
void act(int id);

#endif