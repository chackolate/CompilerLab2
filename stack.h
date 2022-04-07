#include <stdlib.h>

typedef struct stackNode {
  char name[100];
  int val;
  char expression[500];
  struct stackNode *next;
  struct stackNode *prev;
  int tabCnt;
  int cond;
} stackNode;

typedef struct stack {
  struct stackNode *top;
  struct stackNode *bottom;
  int counter;
} stack;

struct stackNode *newNode(char *name, int val);
struct stackNode *findNode(char *name, int val, struct stackNode *node);
struct stackNode *assign(char *name, int val, struct stackNode *head);
struct stackNode *push(struct stack *s, struct stackNode *node);
struct stackNode *pushNew(char *name, int val, struct stack *s);
struct stackNode *pop(struct stack *s);
void stackPrint(struct stack *s);
void freeNodes(struct stackNode *head, struct stack *s);