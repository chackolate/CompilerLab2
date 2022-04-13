#include <stdlib.h>

typedef struct var {
  char name[100];
  int val;
} var;

typedef struct stackNode {
  var var;
  struct stackNode *next;
  char expression[500];
} stackNode;

typedef struct stack {
  stackNode *head;
  stackNode *tail;
  int counter;
} stack;

typedef struct regNode {
  char *name;
  struct regNode **edges;
  int numEdges;
  int firstLine;
  int lastLine;
  int regNum;

} regNode;

// task 1
struct stackNode *createVar(char *name, int val);
struct stackNode *findVar(char *name, int val, stackNode *node, int rec,
                          char *varList);
void recordVar(int rec, char *varList, char *name);
struct stackNode *assignVar(char *name, int val, stackNode *head);
struct stackNode *push(char *name, int val, stack *s);
struct stackNode *pop(stack *s);
void printStack(stack *s);
void task1Main(stack *s);

// task 2
void task2Main(stack *s);
char *readStack(stack *s);
char *findUsrVars(stack *s);

// task 3
void initVars(stack *s);
void printTmps(stack *s);
void printStatements(stack *s);
void printVars(stack *s);
void task3Main(char *usrVars, stack *s);