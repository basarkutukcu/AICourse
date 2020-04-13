#include "environment.h"
#include "agent.h"
#include <stdlib.h>
#include "stdio.h"
#include "math.h"

static struct
{
    int mapSize;
    int numOfAgents;
    int numOfObstacles;
}envParams;

void inputReader()
{
    int i;
    FILE *fp;
    int coordX, coordY;
    fp = fopen("input.txt", "r");
    fscanf(fp,"%d %d %d\n",&envParams.mapSize,&envParams.numOfAgents,&envParams.numOfObstacles);

    env_createGridWorld(envParams.mapSize);
    for(i = 0;i < envParams.numOfObstacles; i++)
    {
        fscanf(fp,"%d %d\n",&coordX, &coordY);
        coordX--;
        coordY--;
        env_setOccupation(coordX, coordY, OCC_OBS);
    }
    
    agn_spawnAgents(envParams.numOfAgents);
    agn_createLocalMap(envParams.mapSize, envParams.numOfAgents);
    env_setOccupation(0, 0, OCC_AGENT);

    fclose(fp);
}

int main()
{
    int i,step;
    inputReader();

    step = 0;
    while(step < 500)
    {
        for(i=0;i<envParams.numOfAgents;i++)
        {
            agn_act(i);
        }
        step++;
    }
    
}