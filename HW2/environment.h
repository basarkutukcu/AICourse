#ifndef ENVIRONMENT_H 
#define ENVIRONMENT_H 

#define OCC_EMPTY   0
#define OCC_AGENT   1
#define OCC_OBS     2

typedef struct GridCells
{
    int occupation;
    int h;
}GridCell;

void env_setOccupation(int x, int y, int occ);
void env_createGridWorld(int n);
int env_getOccupation(int x, int y);

#endif