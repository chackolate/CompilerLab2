#include "varNodeStack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct stackNode *createVar(char *name, int val) {
  struct stackNode *node = (struct stackNode *)malloc(sizeof(struct stackNode));
  sscanf(name, "%s", node->var.name);
  node->var.val = val;
  node->next = NULL;
  return node;
}

struct stackNode *getVar(char *name, int val, struct stackNode *node) {
  if (strcmp(name, node->var.name) == 0) {
    return node;
  } else if (node->next == NULL) {
    node->next = createVar(name, val);
    return node->next;
  } else {
    return getVar(name, val, node->next);
  }
}

struct stackNode *assignVar(char *name, int val, stackNode *head) {
  stackNode *node = getVar(name, val, head);
  node->var.val = val;
  return node;
}

struct stackNode *push(char *name, int val, stack *s) {
  stackNode *node = createVar(name, val);

  if (s->counter != 0) {
    s->tail->next = node;
  } else {
    s->head = node;
  }
  s->tail = node;
  s->counter++;
  return s->tail;
}

struct stackNode *pop(stack *s) {
  stackNode *node = s->tail;
  stackNode *node2 = s->head;
  while (node2->next != node) {
    node2 = node2->next;
  }
  s->tail = node2;
  s->tail->next = NULL;
  s->counter--;
  return node;
}

void printStack(stack *s) {
  stackNode *node = s->head;
  while (node) {
    if (strstr(node->expression, "if(") == NULL) {
      printf("%s%s", node->var.name, node->expression);
    } else {
      printf("%s", node->expression);
    }
    printf("\n");
    node = node->next;
  }
}
