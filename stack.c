#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERBOSE

struct stackNode {
  char *name[100];
  int val;
  char exp[500];
  struct stackNode *next;
  struct stackNode *prev;
  int tabCnt;
  int cond;
};
typedef struct stackNode stackNode;

struct stack {
  stackNode *top;
  stackNode *bottom;
  int counter;
} stack;
typedef struct stack stack;

stackNode *newNode(char *name,
                   int val) { // create a new node to be placed on the stack
  stackNode *node = (stackNode *)malloc(sizeof(stackNode));
  sprintf(node->name, "%s", name);
  node->val = val;
  node->next = NULL;
  node->tabCnt = 0;
  node->cond = 0;
  return node;
}

stackNode *findNode(char *name, int val, stackNode *node) {
  if (!(strcmp(name, node->name))) { // return current node on match
    return node;
  } else if (node->next == NULL) { // end of node list, create node if not found
    node->next = newNode(name, val);
    return node->next;
  } else {
    return findNode(name, val, node->next); // recursively search next node
  }
}

stackNode *assign(char *name, int val, stackNode *head) {
  stackNode *variable = findNode(name, val, head);
  variable->val = val;
  return variable;
}

stackNode *push(stack *s, stackNode *node) {
  if (s->counter != 0) {
    s->top->next = node;
  } else {
    s->bottom = node;
  }
  node->prev = s->top;
  s->top = node;
  s->counter++;
  return s->top;
}

stackNode *pop(stack *s) {
  stackNode *node = s->top;
  s->top = s->top->prev;
  s->counter--;
  return node;
}

void stackPrint(stack *s) {
  stackNode *tmp = s->bottom;
  while (tmp != NULL) {
    for (int i = 0; i < tmp->tabCnt; i++) {
      printf("\t");
    }
    if (tmp->cond) {
      printf("%s", tmp->exp);
    } else {
      printf("%s%s", tmp->name, tmp->exp);
    }
    stackNode *tmp2 = tmp->next;
    free(tmp);
    tmp = tmp2;
  }
  s->bottom = NULL;
  s->top = NULL;
  s->counter = 0;
}

void freeNodes(stackNode *head, stack *s) {
  stackNode *tmp = head->next;
  while (tmp != NULL) {
    stackNode *tmp2 = tmp->next;
    free(tmp);
    tmp = tmp2;
  }
  while (s->bottom != NULL) {
    stackNode *tmp2 = s->bottom->next;
    free(s->bottom);
    s->bottom = tmp2;
  }
  free(s);
  return;
}
