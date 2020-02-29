#include "environment.h"
#include <stdlib.h>
#include "stdio.h"
#include "robot.h"

Robot* robots;
int totalGolds;

void spawnRobots(int n)
{
    int i, j;
    robots = (Robot*)malloc(n * sizeof(Robot*));

    for(i = 0; i < n; i++)
    {
        robots[i].robotID = i;
        robots[i].x = 0;
        robots[i].y = 0;
        robots[i].isCarryGold = 0;
    }
}

void putRobotOnMap(int x, int y, int id)
{
    robots[id].x = x;
    robots[id].y = y;
    setOccupation(x,y,OCCUPATION_ROBOT);
    setIsRobotCarryGold(x, y, robots[id].isCarryGold);
    setRobotID(x, y, id);
}

void setTotalGolds(int totGold)
{
    totalGolds = totGold;
}

int getTotalGolds()
{
    return totalGolds;
}

void pickUpGold(int x,int y,int id)
{
    int goldNum;

    goldNum = getGoldNum(x,y);
    if(goldNum < 1)
    {
        printf("No Gold to pick up in this cell x: %d - y: %d", x, y);
        return;
    }
    goldNum--;
    setGoldNum(x, y, goldNum);
    robots[id].isCarryGold = 1;
}

void putDownGold(int x, int y, int id)
{
    int goldNum;
    if(robots[id].isCarryGold == 0)
    {
        printf("Robot has no gold to put down");
        return;
    }
    goldNum = getGoldNum(x,y);
    goldNum++;
    setGoldNum(x, y, goldNum);
    robots[id].isCarryGold = 0;
    totalGolds--;
}

void move(int dirX, int dirY, int id)
{
    if(getOccupation(robots[id].x, robots[id].y) == OCCUPATION_ROBOT)   // Robot cannot overwrite D/G
        setOccupation(robots[id].x, robots[id].y, OCCUPATION_NONE);
    
    robots[id].x = robots[id].x + dirX;
    robots[id].y = robots[id].y + dirY;
    setIsRobotCarryGold(robots[id].x, robots[id].y, robots[id].isCarryGold);
    if(getOccupation(robots[id].x, robots[id].y) == OCCUPATION_NONE)
        setOccupation(robots[id].x, robots[id].y, OCCUPATION_ROBOT);
}

void act(int id)
{
    int currX, currY, currOcc, isCarryGold;

    currX = robots[id].x;
    currY = robots[id].y;
    currOcc = getOccupation(currX, currY);
    isCarryGold = robots[id].isCarryGold;

    if(isCarryGold == 1 && currOcc == OCCUPATION_DEPOT)
    {
        putDownGold(currX, currY, id);
    }
    else if(isCarryGold == 0 && currOcc == OCCUPATION_GOLD)
    {
        pickUpGold(currX, currY, id);
    }
    else if(isCarryGold == 1)
    {
        move(-1,0,id);
    }
    else if(isCarryGold == 0)
    {
        move(1,0,id);
    }
}