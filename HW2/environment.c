#include "environment.h"
#include <stdlib.h>
#include "stdio.h"
#include "math.h"


GridCell** globalMap;

void env_createGridWorld(int n)
{
    int i, j;

    globalMap = (GridCell **)malloc(n * sizeof(GridCell*));
    for(i = 0;i<n;i++)
    {
        globalMap[i] = (GridCell *)malloc(n * sizeof(GridCell));
    }

    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n; j++)
        {
            globalMap[i][j].occupation = OCC_EMPTY;
            globalMap[i][j].h = 1;
        }
    }
    globalMap[n-1][n-1].h = 0;
}

void env_setOccupation(int x, int y, int occ)
{
    globalMap[x][y].occupation = occ;
}

int env_getOccupation(int x, int y)
{
    return globalMap[x][y].occupation;
}

void env_sethG(int x, int y, int val)
{
    globalMap[x][y].h = val;
}

int env_gethG(int x, int y)
{
    return globalMap[x][y].h;
}