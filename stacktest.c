#include "stack.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct stackNode *head;
struct stack *s;

int main() {
  head = (struct stackNode *)malloc(sizeof(struct stackNode));
  s = (struct stack *)malloc(sizeof(struct stack));
  s->top = NULL;
  s->bottom = NULL;
  s->counter = 0;
  struct stackNode *node = newNode("tmp", 0);
}