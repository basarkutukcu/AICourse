#ifndef AGENT_H 
#define AGENT_H 

#define FVAL_INVALID    99999
#define FVAL_INF        9999

typedef struct LocalGridCells
{
    int h;
    int isVisited;
    int isAgent;
}LocalGridCell;

typedef struct Agents
{
    int x;
    int y;
}Agent;

void agn_createLocalMap(int n, int r);
int agn_gethL(int a, int x, int y);
void agn_sethL(int a, int x, int y, int val);
void agn_act(int a);
void agn_lookAheadSearch(int a);
void agn_spawnAgents(int n);
int agn_getIsVisited(int a, int x, int y);
void agn_setIsVisited(int a, int x, int y, int val);
void agn_calculateF(int a, int x, int y, int findex);
int agn_isValid(int x, int y);
void agn_findMinF();
void agn_move(int a, int c);
void agn_estUpdate(int a);
int agn_repulsiveChoice(int a);
int agn_getTermSig();

#endif