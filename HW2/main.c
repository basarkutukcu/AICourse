#include "environment.h"
#include "agent.h"
#include <stdlib.h>
#include "stdio.h"
#include "math.h"
#include <time.h>

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

clock_t start, end;
double cpu_time_used;

int main()
{
    int i,step;
    inputReader();

    step = 0;
    start = clock();
    while(step < 50000)
    {
        for(i=0;i<envParams.numOfAgents;i++)
        {
            agn_act(i);
            if(agn_getTermSig() == 1)
            {
                end = clock();
                cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
                printf("in %f seconds\n\n",cpu_time_used);
                exit(1);
            }
        }
        step++;
        
    }
    
}