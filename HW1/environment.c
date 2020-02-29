#include "environment.h"
#include <stdlib.h>
#include "stdio.h"

GridCell** gridWorld;
int gridLen;

void createGridWorld(int n)
{
    int i, j;
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
            gridWorld[i][j].clusterNum = 0;
            gridWorld[i][j].goldNum = 0;
            gridWorld[i][j].isRobotCarryGold = 0;
            gridWorld[i][j].occupation = 0;
        }
    }
}

int getClusterNum(int x, int y)
{
    return gridWorld[x][y].clusterNum;
}

void setClusterNum(int x, int y, int val)
{
    gridWorld[x][y].clusterNum = val;
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
}

void printCurrGrid()
{
    int x,y;

    for ( x = 0; x < gridLen; x++)
    {
        for ( y = 0; y < gridLen; y++)
        {
            if(gridWorld[x][y].occupation == OCCUPATION_NONE)
            {
                printf("x  ");
            }
            else if (gridWorld[x][y].occupation == OCCUPATION_DEPOT)
            {
                printf("D");
                printf("%d ",gridWorld[x][y].goldNum);
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
                // TODO: add robot id
                
            }
            
            
            
        }
        printf("\n");
    }
}



