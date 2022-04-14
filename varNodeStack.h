#include <stdbool.h>
#include <stdlib.h>

typedef struct var {
  char name[100];
  int val;
} var;

typedef struct stackNode {
  var var;
  struct stackNode *next;
  struct stackNode *prev;
  char expression[500];
} stackNode;

typedef struct stack {
  stackNode *head;
  stackNode *tail;
  int counter;
} stack;

typedef struct regNode {
  char name[100];
  int firstPos;
  int lastPos;
} regNode;

typedef struct regNodeEdge {
  regNode src;
  regNode dst;
} regNodeEdge;

typedef struct graphNode {
  char *src;
  char **dsts;
  regNodeEdge *edges;
  int numEdges;
  int regNum;
} graphNode;

typedef struct registerFile {
  regNode reg;
  int occupied;
} registerFile;

// typedef struct regNodeGraph {
//   struct regNode *head;
//   regNodeEdge **edges;
//   int numVars;
//   int numEdges;
// } regNodeGraph;

typedef struct nodeGraph {
  graphNode **nodes; // 2d array of nodes that are put together to form a graph
  int numNodes;
} nodeGraph;

// task 1
struct stackNode *createVar(char *name, int val);
struct stackNode *findVar(char *name, int val, stackNode *node, int rec,
                          char *varList);
void recordVar(int rec, char *varList, char *name);
struct stackNode *assignVar(char *name, int val, stackNode *head);
struct stackNode *push(char *name, int val, stack *s);
void pushNode(stackNode *node, stack *s);
struct stackNode *pop(stack *s);
void printStack(stack *s);
void task1Main(stack *s);

// task 2
void task2Main(stack *s);
char **findUsrVars(stack *s, int *size, char **varList);
char *readStack(stack *s);
void divideStack(stack *s, char **instructions, int *size);
int stackLength(stack *s);
int findFirstLine(char **instructions, int lineCnt, char *usrVar);
int findLastLine(char **instructions, int lineCnt, char *usrVar);
void fillNodes(regNode *vars, char **varsBuf, char **instructions, int lineCnt,
               int numVars);
bool checkLive(regNode var1, regNode var2, int *bound1, int *bound2);
void createEdgeArray(regNode var, regNode *vars, int numVars,
                     regNodeEdge *edges, int *numEdges);
regNodeEdge *createEdges(regNode *vars, regNodeEdge *edges, int numVars,
                         int *edgeCnt);
graphNode *createGraphNode(regNode *vars, regNode *src, int numVars,
                           regNodeEdge *edges, int *numEdges);
// regNodeGraph *createGraph(regNodeGraph *graph, regNodeEdge **edges,
//                           int edgeCnts, regNode *vars, int numVars);
nodeGraph *createGraph(graphNode **nodes, int numNodes);
void printGraph(nodeGraph *graph, int numVars);
void assignRegs(stack *s, regNode *vars, int numVars);
bool isInRegs(char *name, registerFile *regs);
bool storeRegs(registerFile *regs, regNode reg, stack *s);
bool clearRegs(registerFile *regs, regNode reg, stack *s);

// task 3
void printVars(stack *s);
void printTmps(stack *s);
void printUsrVars(stack *s, char *usrVars);
void printStatements(stack *s);
void reprintVars(stack *s);
void task3Main(char *usrVars, stack *s);