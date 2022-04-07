#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERBOSE

struct stackNode *
newNode(char *name,
        int val) { // create a new node to be placed on the stack
  struct stackNode *node = (struct stackNode *)malloc(sizeof(struct stackNode));
  sprintf(node->name, "%s", name);
  node->val = val;
  sprintf(node->expression, NULL);
  node->next = NULL;
  node->prev = NULL;
  node->tabCnt = 0;
  node->cond = 0;
  return node;
}

struct stackNode *findNode(char *name, int val, struct stackNode *node) {
  if (!(strcmp(name, node->name))) { // return current node on match
    return node;
  } else if (node->next == NULL) { // end of node list, create node if not found
    node->next = newNode(name, val);
    return node->next;
  } else {
    return findNode(name, val, node->next); // recursively search next node
  }
}

struct stackNode *assign(char *name, int val, struct stackNode *head) {
  struct stackNode *variable = findNode(name, val, head);
  variable->val = val;
  return variable;
}

struct stackNode *push(struct stack *s, struct stackNode *node) {
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

struct stackNode *pushNew(char *name, int val, struct stack *s) {
  struct stackNode *node = newNode(name, val);
  return push(s, node);
}

struct stackNode *pop(struct stack *s) {
  struct stackNode *node = s->top;
  s->top = s->top->prev;
  s->counter--;
  return node;
}

void stackPrint(struct stack *s) {
  printf("printing stack\n");
  struct stackNode *tmp = s->bottom;
  while (tmp != NULL) {
    for (int i = 0; i < tmp->tabCnt; i++) {
      printf("\t");
    }
    if (tmp->cond) {
      printf("%s\n", tmp->expression);
    } else {
      printf("%s%s\n", tmp->name, tmp->expression);
    }
    struct stackNode *tmp2 = tmp->next;
    free(tmp);
    tmp = tmp2;
  }
  s->bottom = NULL;
  s->top = NULL;
  s->counter = 0;
}

void freeNodes(struct stackNode *head, struct stack *s) {
  struct stackNode *tmp = head->next;
  while (tmp != NULL) {
    struct stackNode *tmp2 = tmp->next;
    free(tmp);
    tmp = tmp2;
  }
  while (s->bottom != NULL) {
    struct stackNode *tmp2 = s->bottom->next;
    free(s->bottom);
    s->bottom = tmp2;
  }
  free(s);
  return;
}
