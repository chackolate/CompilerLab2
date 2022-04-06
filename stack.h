#include <stdlib.h>

stackNode *newNode(char *name, int val);
stackNode *findNode(char *name, int val, stackNode *node);
stackNode *assign(char *name, int val, stackNode *head);
stackNode *push(stack *s, stackNode *node);
stackNode *pop(stack *s);
void stackPrint(stack *s);
void freeNodes(stackNode *head, stack *s)