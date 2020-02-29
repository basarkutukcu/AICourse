#include "stdio.h"
#include "environment.h"
#include "stdlib.h"
#include "robot.h"


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
    int coordX, coordY, numOfGolds, robotID;
    fp = fopen("env.inp", "r");
    fscanf(fp,"%d\n",&envParams.gridSize);
    createGridWorld(envParams.gridSize);
    fscanf(fp,"%d\n",&envParams.numOfRobots);
    spawnRobots(envParams.numOfRobots);
    fscanf(fp,"%d\n",&envParams.numOfGoldClusters);
    fscanf(fp,"%d\n",&envParams.numOfObstacles);

    robotID = 0;
    while (objType != 'd')
    {
        fscanf(fp,"%c ",&objType);

        if(objType == 'r')
        {
            fscanf(fp,"%d ",&coordX);
            fscanf(fp,"%d\n",&coordY);
            putRobotOnMap(coordX, coordY, robotID);
            robotID++;// Use these to spawn robots
        }
        else if(objType == 'g')
        {
            fscanf(fp,"%d ",&coordX);
            fscanf(fp,"%d ",&coordY);
            fscanf(fp,"%d\n",&numOfGolds);
            setOccupation(coordX,coordY,OCCUPATION_GOLD);
            setGoldNum(coordX,coordY,numOfGolds);
            addToTotalGolds(numOfGolds);
        }
        else if(objType == 'o')
        {
            fscanf(fp,"%d ",&coordX);
            fscanf(fp,"%d\n",&coordY);
            setOccupation(coordX,coordY,OCCUPATION_OBSTACLE);
        }
        else if(objType == 'd')
        {
            fscanf(fp,"%d ",&coordX);
            fscanf(fp,"%d\n",&coordY);
            setOccupation(coordX,coordY,OCCUPATION_DEPOT);
            setGoldNum(coordX,coordY,0);
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
    int simStep;
    int i;
    inputReader();
    calculateSignalStr();
    printCurrGrid();

    simStep = 0;
    while(getTerminateSignal() == 0 && simStep < 500)
    {
        printf("sim step: %d \n", simStep);
        for(i = 0; i < envParams.numOfRobots; i++)
        {
            act(i);
        }
        printCurrGrid();
        printf("\n\n");
        simStep++;
    }

    return 0;
}