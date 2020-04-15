#include "environment.h"
#include "agent.h"
#include <stdlib.h>
#include "stdio.h"
#include "math.h"

LocalGridCell*** localMap;

// f[0] - Left
// f[1] - Up
// f[2] - Right
// f[3] - Down
int f[4];
Agent* agents;
int numOfAgents;
int mapLen;
int minIndex;
int secondMinIndex;
int minAvailables[4];
int terminateSignal;

void agn_spawnAgents(int n)
{
    int i, j;
    agents = (Agent*)malloc(n * sizeof(Agent));
    numOfAgents = n;
    terminateSignal = 0;

    for(i = 0; i < n; i++)
    {
        agents[i].x = 0;
        agents[i].y = 0;
    }
}

void agn_createLocalMap(int n, int r)
{
    int i, j, k;
    mapLen = n;

    localMap = (LocalGridCell ***)malloc(r * sizeof(LocalGridCell**));
    for(i=0;i<r;i++)
    {
        localMap[i] = (LocalGridCell **)malloc(n * sizeof(LocalGridCell*));
    }
    for(i = 0;i<r;i++)
    {
        for(j = 0;j<n;j++)
        {
            localMap[i][j] = (LocalGridCell *)malloc(n * sizeof(LocalGridCell));
        }
    }

    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n; j++)
        {
            for(k=0;k<r;k++)
            {
                localMap[k][i][j].h = 1;
                localMap[k][i][j].isVisited = 0;
                localMap[k][i][j].isAgent = 0;
            }
            
        }
    }
}

int agn_gethL(int a, int x, int y)
{
    return localMap[a][x][y].h;
}

void agn_sethL(int a, int x, int y, int val)
{
    localMap[a][x][y].h = val;
}

int agn_getIsVisited(int a, int x, int y)
{
    return localMap[a][x][y].isVisited;
}

void agn_setIsVisited(int a, int x, int y, int val)
{
    localMap[a][x][y].isVisited = val;
}

void agn_act(int a)
{
    int choice;
    agn_lookAheadSearch(a);
    agn_findMinF();
    choice = agn_repulsiveChoice(a);
    agn_estUpdate(a);
    agn_move(a, choice);
}

void agn_lookAheadSearch(int a)
{
    int currX,currY,x,y;

    currX = agents[a].x;
    currY = agents[a].y;

    x = currX - 1;
    y = currY;
    agn_calculateF(a,x,y,0);

    x = currX;
    y = currY + 1;
    agn_calculateF(a,x,y,1);

    x = currX + 1;
    y = currY;
    agn_calculateF(a,x,y,2);

    x = currX;
    y = currY - 1;
    agn_calculateF(a,x,y,3);
    
}

void agn_calculateF(int a, int x, int y, int findex)
{
    if(agn_isValid(x,y))
    {
        if(localMap[a][x][y].isVisited == 1)
        {
            f[findex] = 1 + localMap[a][x][y].h;
        }
        else
        {
            f[findex] = 1 + env_gethG(x,y);
        }
    }
    else
    {
        f[findex] = FVAL_INVALID;
    }
}

int agn_agentCoordX(int a, int c)
{
    if(c == 0)
        return agents[a].x - 1;
    else if(c == 1 || c == 3)
        return agents[a].x;
    else if(c == 2)
        return agents[a].x + 1;
}

int agn_agentCoordY(int a, int c)
{
    if(c == 3)
        return agents[a].y - 1;
    else if(c == 0 || c == 2)
        return agents[a].y;
    else if(c == 1)
        return agents[a].y + 1;
}

float agn_calculateAdj(int a, int c)
{
    int currX,currY,i,minAdjIndex;
    float adj[200];
    float min = 9999;
    currX = agn_agentCoordX(a,c);
    currY = agn_agentCoordY(a,c);

    for ( i = 0; i < numOfAgents; i++)
    {
        if(i == a)
        {
            adj[i] = 99999;
            continue;
        }
        adj[i] = sqrtf(powf((currX - agents[i].x),2) + powf((currY - agents[i].y),2));
        if(adj[i] < min)
        {
            min = adj[i];
            minAdjIndex = i;
        }
    }
    return min;
}


int agn_isValid(int x, int y)
{
    if(x > mapLen-1 || y > mapLen-1 || x<0 || y<0)
        return 0;

    if(env_getOccupation(x,y) == OCC_OBS)
        return 0;

    return 1;
}

void agn_findMinF()
{
    int i;
    int min = FVAL_INF;
    minIndex = (rand() % 4);
    secondMinIndex = minIndex;

    for(i = 0;i < 4; i++)
    {
        if( f[i] < min)
        {
            min = f[i];
            minIndex = i;
            secondMinIndex = minIndex;
        }
        minAvailables[i] = 0;
    }
    for ( i = 0; i < 4; i++)
    {
        if(f[i] == min)
            minAvailables[i] = 1;
    }
    
}

int agn_repulsiveChoice(int a)
{
    int i;
    float adjVal;
    float maxAdj = 0;
    int maxAdjIndex;
    float alpha = 1.5;
    float rVal = (float) (alpha * localMap[a][agents[a].x][agents[a].y].h);
    for ( i = 0; i < 4; i++)
    {
        if(minAvailables[i] == 1)
        {
            adjVal = agn_calculateAdj(a,i);
            if(adjVal > rVal)
            {
                return i;
            }
            else
            {
                if(adjVal > maxAdj)
                {
                    maxAdj = adjVal;
                    maxAdjIndex = i;
                }
            }
            
        }
    }
    return maxAdjIndex;    
}

void agn_move(int a, int c)
{
    printf("Agent%d: ",a);
    localMap[a][agents[a].x][agents[a].y].isVisited = 1;
    if(c == 0)
    {
        agents[a].x = agents[a].x - 1;
        agents[a].y = agents[a].y;
        printf("L");
    }
    else if (c == 1)
    {
        agents[a].x = agents[a].x;
        agents[a].y = agents[a].y + 1;
        printf("U");
    }
    else if (c == 2)
    {
        agents[a].x = agents[a].x + 1;
        agents[a].y = agents[a].y;
        printf("R");
    }
    else if (c == 3)
    {
        agents[a].x = agents[a].x;
        agents[a].y = agents[a].y - 1;
        printf("D");
    }
    else
    {
        printf("Wrong c Index \n\n");
    }
    printf("    (%d,%d)\n",agents[a].x+1,agents[a].y+1);
    if(agents[a].x == mapLen-1 && agents[a].y == mapLen-1)
    {
        printf("Agent%d reached the goal ",a);
       terminateSignal = 1;
    }
}

int agn_getTermSig()
{
    return terminateSignal;
}

void agn_estUpdate(int a)
{
    int x = agents[a].x;
    int y = agents[a].y;
    env_sethG(x, y, f[minIndex]);
    localMap[a][x][y].h = f[secondMinIndex];
}