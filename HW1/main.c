#include "stdio.h"
#include "environment.h"
#include "stdlib.h"


static struct
{
    int gridSize;
    int numOfRobots;
    int numOfGoldClusters;
    int numOfObstacles;
}envParams;

void inputReader();

void inputReader()
{
    char objType;
    FILE *fp;
    fp = fopen("env.inp", "r");
    fscanf(fp,"%d",&envParams.gridSize);
    fscanf(fp,"\n");
    fscanf(fp,"%d",&envParams.numOfRobots);
    fscanf(fp,"\n");
    fscanf(fp,"%d",&envParams.numOfGoldClusters);
    fscanf(fp,"\n");
    fscanf(fp,"%d",&envParams.numOfObstacles);
    fscanf(fp,"\n");

    while (objType != 'd')
    {
        fscanf(fp,"%c",&objType);
        fscanf(fp," ");

        if(objType == 'r')
        {

        }
        else if(objType == 'g')
        {

        }
        else if(objType == 'o')
        {

        }
        else if(objType == 'd')
        {

        }
        else
        {
            printf("Unknown Object type in env.inp \n");
        }
        
    }
    
    fclose(fp);
}

int main()
{
    int a;
    printf("xy\n");
    createGridWorld(10);

    a = getClusterNum(3,4);
    printf("a: %d \n",a);

    setClusterNum(3,4,8);
    a = getClusterNum(3,4);
    printf("a: %d \n",a);

    printCurrGrid();

    return 0;
}