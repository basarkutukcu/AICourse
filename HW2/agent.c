#include "environment.h"
#include "agent.h"
#include <stdlib.h>
#include "stdio.h"

LocalGridCell*** localMap;

// f[0] - Left
// f[1] - Up
// f[2] - Right
// f[3] - Down
int f[4];

void agn_createLocalMap(int n, int r)
{
    int i, j, k;

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
    
}

void agn_lookAheadSearch(int a)
{
    
}