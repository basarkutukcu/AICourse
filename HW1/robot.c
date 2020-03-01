#include "environment.h"
#include <stdlib.h>
#include "stdio.h"
#include "robot.h"

#define MAX_RANDOM_MOVE 4

Robot* robots;
int randomMoveNum;

void spawnRobots(int n)
{
    int i, j;
    robots = (Robot*)malloc(n * sizeof(Robot*));
    randomMoveNum = 0;  // init

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
    if(goldNum == 0)
    {
        setOccupation(x,y,OCCUPATION_NONE);
    }
    else
    {
        setGoldNum(x, y, goldNum);
    }    
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
}

void move(int dirX, int dirY, int id)
{
    if(getOccupation(robots[id].x, robots[id].y) == OCCUPATION_ROBOT)   // Robot cannot overwrite D/G
        setOccupation(robots[id].x, robots[id].y, OCCUPATION_NONE);
    
    robots[id].x = robots[id].x + dirX;
    robots[id].y = robots[id].y + dirY;
    setIsRobotCarryGold(robots[id].x, robots[id].y, robots[id].isCarryGold);
    setRobotID(robots[id].x, robots[id].y, id);
    if(getOccupation(robots[id].x, robots[id].y) == OCCUPATION_NONE)
        setOccupation(robots[id].x, robots[id].y, OCCUPATION_ROBOT);
}

int validCoord(int x, int y)
{
    int occ, gridLen;

    gridLen = getGridLen();
    if(x > gridLen-1 || y > gridLen-1 || x<0 || y<0)
        return 0;

    occ = getOccupation(x,y);
    if(occ == OCCUPATION_NONE || occ == OCCUPATION_DEPOT || occ == OCCUPATION_GOLD)
    {
        return 1;
    }
    else
    {
        return 0;
    }
    
}

coords findValidRandom(int curX, int curY)
{
    int randX, randY, tempX, tempY, isValid;
    coords validRandCoords;
    isValid = 0;
    while (isValid == 0)
    {
        randX = (rand() % 3) - 1;
        randY = (rand() % 3) - 1;
        tempX = randX + curX;
        tempY = randY + curY;

        isValid = validCoord(tempX, tempY);
    }
    validRandCoords.x = randX;
    validRandCoords.y = randY;
    return validRandCoords;    
}

coords findValidMinMax(int curX, int curY, int type)
{
    int x,y;
    coords validSrcCoords;
    int minX = 0, minY = 0;
    int maxX = 0, maxY = 0;
    float minVal, maxVal, currVal;
    minVal = 9999;
    maxVal = -1;
    for ( x = -1; x < 2; x++)
    {
        for ( y = -1; y < 2; y++)
        {
            if(validCoord(curX + x, curY + y) == 1)
            {
                currVal = getSignalStr(curX + x, curY + y);
                if(currVal < minVal)
                {
                    minVal = currVal;
                    minX = x;
                    minY = y;
                }
                if(currVal > maxVal)
                {
                    maxVal = currVal;
                    maxX = x;
                    maxY = y;
                }
            }

        }
    }
    if(type == SEARCH_MIN)
    {
        validSrcCoords.x = minX;
        validSrcCoords.y = minY;
    }
    else
    {
        validSrcCoords.x = maxX;
        validSrcCoords.y = maxY;
    }
    return validSrcCoords;
}

void leaveCrumbs(int x, int y)
{
    int crNum;
    crNum = getcrumbNum(x,y);
    crNum = crNum + 2;
    setcrumbNum(x,y,crNum);
}

void takeCrumbs(int x, int y)
{
    int crNum;
    crNum = getcrumbNum(x,y);
    crNum = crNum - 1;
    setcrumbNum(x,y,crNum);
}

void followCrumbs(int curX, int curY, int id)
{
    int x, y;
    int crumbX = 0, crumbY = 0;
    int tempX, tempY;
    float maxSignal = 0;
    int isValid;

    for ( x = -1; x < 2; x++)
    {
        for ( y = -1; y < 2; y++)
        {
            if(validCoord(curX + x, curY + y) == 1)
            {
                if(x == 0 && y == 0)    // don't check current cell
                    break;

                tempX = curX + x;
                tempY = curY + y;
                if(getcrumbNum(tempX, tempY) > 0)   // Neighboring crumb found
                {
                    if(getSignalStr(tempX, tempY) > maxSignal)
                    {
                        crumbX = x;
                        crumbY = y;
                        maxSignal = getSignalStr(tempX, tempY);
                    }
                }
            }
        }
    }

    isValid = validCoord(curX + crumbX, curY + crumbY);
    if(isValid)
        move(crumbX, crumbY , id);
    
}

int senseGold(int curX, int curY)
{
    int x, y;

    for ( x = -1; x < 2; x++)
    {
        for ( y = -1; y < 2; y++)
        {
            if(validCoord(curX + x, curY + y) == 1)
            {
                if(getOccupation(curX+x,curY+y) == OCCUPATION_GOLD)
                {
                    return 1;
                }
            }
            
        }
    }
    return 0;
}

coords senseGoldCoords(int curX, int curY)
{
    int x, y;
    coords goldCoords;
    goldCoords.x = 0;
    goldCoords.y = 0;
    for ( x = -1; x < 2; x++)
    {
        for ( y = -1; y < 2; y++)
        {
            if(validCoord(curX + x, curY + y) == 1)
            {
                if(getOccupation(curX+x,curY+y) == OCCUPATION_GOLD)
                {
                    goldCoords.x = x;
                    goldCoords.y = y;
                }
            }
            
        }
    }
    return goldCoords;
}

void act(int id)
{
    int currX, currY, currOcc, isCarryGold;
    coords validCoords;
    coords goldCoords;

    printf("id : %d \n",id);
    currX = robots[id].x;
    currY = robots[id].y;
    currOcc = getOccupation(currX, currY);
    isCarryGold = robots[id].isCarryGold;

    if(isCarryGold == 1 && currOcc == OCCUPATION_DEPOT) // Drop Gold
    {
        putDownGold(currX, currY, id);
    }
    else if(isCarryGold == 1)   // Follow Gradient to Depot
    {
        validCoords = findValidMinMax(currX, currY, SEARCH_MIN);
        leaveCrumbs(currX,currY);
        move(validCoords.x,validCoords.y,id);
    }
    else if(isCarryGold == 0 && currOcc == OCCUPATION_GOLD) // Pick Gold
    {
        pickUpGold(currX, currY, id);
    }
    else if(isCarryGold == 0 && senseGold(currX, currY) == 1)   // Go into neighbor gold cluster
    {
        goldCoords = senseGoldCoords(currX,currY);
        move(goldCoords.x, goldCoords.y, id);
    }
    else if (isCarryGold == 0 && getcrumbNum(currX, currY) > 0) // Follow crumbs to Gold Cluster
    {
        takeCrumbs(currX, currY);
        followCrumbs(currX, currY, id);
    }
    
    else if(isCarryGold == 0)   // Wander
    {
        if(randomMoveNum < MAX_RANDOM_MOVE)
        {
            validCoords = findValidRandom(currX, currY);
            randomMoveNum++;
        }
        else
        {
            validCoords = findValidMinMax(currX, currY, SEARCH_MAX);
            randomMoveNum = 0;
        }
        move(validCoords.x,validCoords.y,id);
    }
    else
    {
        // Do nothing
    }
    
}