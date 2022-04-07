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
struct stackNode *getVar(char *name, int val, stackNode *node);
struct stackNode *assignVar(char *name, int val, stackNode *head);
struct stackNode *push(char *name, int val, stack *s);
struct stackNode *pop(stack *s);
void printStack(stack *s);