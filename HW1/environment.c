#include "environment.h"
#include <stdlib.h>
#include "stdio.h"
#include "math.h"

GridCell** gridWorld;
int gridLen;
int totalGolds;
int terminateSignal;
int depotX;
int depotY;

void createGridWorld(int n)
{
    int i, j;

    totalGolds = 0;
    terminateSignal = 0;

    gridLen = n;
    gridWorld = (GridCell **)malloc(n * sizeof(GridCell*));
    for(i = 0;i<n;i++)
    {
        gridWorld[i] = (GridCell *)malloc(n * sizeof(GridCell));
    }

    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n; j++)
        {
            gridWorld[i][j].goldNum = 0;
            gridWorld[i][j].isRobotCarryGold = 0;
            gridWorld[i][j].occupation = 0;
            gridWorld[i][j].crumbNum = 0;
        }
    }
}

int getGridLen()
{
    return gridLen;
}

int getGoldNum(int x, int y)
{
    return gridWorld[x][y].goldNum;
}

void setGoldNum(int x, int y, int val)
{
    gridWorld[x][y].goldNum = val;
}

int getIsRobotCarryGold(int x, int y)
{
    return gridWorld[x][y].isRobotCarryGold;
}

void setIsRobotCarryGold(int x, int y, int val)
{
    gridWorld[x][y].isRobotCarryGold = val;
}

int getOccupation(int x, int y)
{
    return gridWorld[x][y].occupation;
}

void setOccupation(int x, int y, int val)
{
    gridWorld[x][y].occupation = val;
    if(val == OCCUPATION_DEPOT)
    {
        depotX = x;
        depotY = y;
    }
}

int getRobotID(int x, int y)
{
    return gridWorld[x][y].robotID;
}

void setRobotID(int x, int y, int val)
{
    gridWorld[x][y].robotID = val;
}

int getcrumbNum(int x, int y)
{
    return gridWorld[x][y].crumbNum;
}

void setcrumbNum(int x, int y, int val)
{
    gridWorld[x][y].crumbNum = val;
}

void setTerminateSignal()
{
    terminateSignal = 1;
}

int getTerminateSignal()
{
    return terminateSignal;
}

void addToTotalGolds(int val)
{
    totalGolds = totalGolds + val;
}

void calculateSignalStr()
{
    int x,y;
    for ( x = 0; x < gridLen; x++)
    {
        for ( y = 0; y < gridLen; y++)
        {
            gridWorld[x][y].distanceToDepot = sqrtf(powf((x - depotX),2) + powf((y - depotY),2));
        }
        
    }
}

float getSignalStr(int x, int y)
{
    return gridWorld[x][y].distanceToDepot;
}

void printCurrGrid()
{
    int x,y;

    for ( y = 0; y < gridLen; y++)
    {
        for ( x = 0; x < gridLen; x++)
        {
            if(gridWorld[x][y].occupation == OCCUPATION_NONE)
            {
                printf("x  ");
            }
            else if (gridWorld[x][y].occupation == OCCUPATION_DEPOT)
            {
                printf("D");
                printf("%d ",gridWorld[x][y].goldNum);
                if(totalGolds == gridWorld[x][y].goldNum)
                    setTerminateSignal();
            }
            else if (gridWorld[x][y].occupation == OCCUPATION_GOLD)
            {
                printf("G");
                printf("%d ",gridWorld[x][y].goldNum);
            }
            else if (gridWorld[x][y].occupation == OCCUPATION_OBSTACLE)
            {
                printf("O  ");
            }
            else if(gridWorld[x][y].occupation == OCCUPATION_ROBOT)
            {
                if(gridWorld[x][y].isRobotCarryGold == 1)
                {
                    printf("R");
                }
                else
                {
                    printf("r");
                }
                printf("%d ",gridWorld[x][y].robotID);
            }
        }
        printf("\n");
    }
}
