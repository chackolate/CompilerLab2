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

struct stackNode *createVar(char *name, int val);
struct stackNode *findVar(char *name, int val, stackNode *node, int rec,
                          char *varList);
void recordVar(int rec, char *varList, char *name);
struct stackNode *assignVar(char *name, int val, stackNode *head);
struct stackNode *push(char *name, int val, stack *s);
struct stackNode *pop(stack *s);
void printStack(stack *s);
void initVars(stack *s);
void initTmps(stack *s);
void printStatements(stack *s);
void printVars(stack *s);
void task3Main(char *usrVars, stack *s);