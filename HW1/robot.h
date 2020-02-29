#ifndef ROBOT_H 
#define ROBOT_H 

#define SEARCH_MIN  0
#define SEARCH_MAX  1

typedef struct Robots
{
    int robotID;
    int x;
    int y;
    int isCarryGold;
}Robot;

typedef struct crds
{
    int x;
    int y;
}coords;

void spawnRobots(int n);
void putRobotOnMap(int x, int y, int id);
void act(int id);

#endif