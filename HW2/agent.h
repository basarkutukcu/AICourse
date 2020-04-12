#ifndef AGENT_H 
#define AGENT_H 

typedef struct LocalGridCells
{
    int h;
    int isVisited;
    int isAgent;
}LocalGridCell;

void agn_createLocalMap(int n, int r);
int agn_gethL(int a, int x, int y);
void agn_sethL(int a, int x, int y, int val);
void agn_act(int a);
void agn_lookAheadSearch(int a);

#endif