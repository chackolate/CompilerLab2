#include "varNodeStack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct stackNode *createVar(char *name, int val) { // create a new user variable
  struct stackNode *node = (struct stackNode *)malloc(sizeof(struct stackNode));
  sscanf(name, "%s", node->var.name);
  node->var.val = val;
  node->next = NULL;
  return node;
}

struct stackNode *
findVar(char *name, int val, struct stackNode *node, int rec,
        char *varList) { // if variable is to be newly assigned, add it to the
                         // list of user vars.
  if (strcmp(name, node->var.name) == 0) {
    return node;
  } else if (node->next == NULL) {
    node->next = createVar(name, val);
    if (rec == 1) {
      strcat(varList, name);
      strcat(varList, " ");
    }
    return node->next;
  } else {
    return findVar(name, val, node->next, rec, varList);
  }
}

void recordVar(int rec, char *varList, char *name) {
  if (rec == 1) {
    strcat(varList, name);
    strcat(varList, " ");
  }
}

struct stackNode *assignVar(char *name, int val, stackNode *head) {
  char vars[100];
  stackNode *node = findVar(name, val, head, 1, vars);
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

void initVars(stack *s) {
  char vars[1000];
  sprintf(vars, "\tint");
  stackNode *node = s->head;
  char nameBuf[100];
  while (strstr(node->var.name, "tmp") !=
         NULL) { // check if first element is temp or var (need to print without
                 // a comma)
    node = node->next;
  }
  if (node) { // first element print with no comma
    if (strstr(node->var.name, "tmp") ==
        NULL) { // if tmp is found in a name, it is a tmp. add to list
      // printf("condition 1\n");
      sprintf(nameBuf, " %s", node->var.name);
      strcat(vars, nameBuf);
    }
    node = node->next;
  }
  while (node) { // consecutive elements print with comma
    if (strstr(node->var.name, "tmp") == NULL) {
      // printf("condition 2\n");
      sprintf(nameBuf, ", %s", node->var.name);
      strcat(vars, nameBuf);
    }
    node = node->next;
  }
  strcat(vars, ";\n");
  printf("\n%s", vars);
}

void initTmps(stack *s) {
  char tmps[1000];
  sprintf(tmps, "\tint");
  stackNode *node = s->head;
  char nameBuf[100];
  while (strstr(node->var.name, "tmp") ==
         NULL) { // check if first element is temp or var (need to print without
                 // a comma)
    node = node->next;
  }
  if (node != NULL) {
    if (strstr(node->var.name, "tmp") !=
        NULL) { // if tmp is found in a name, it is a tmp. add to list
      sprintf(nameBuf, " %s", node->var.name);
      strcat(tmps, nameBuf);
    }
    node = node->next;
  }
  while (node) {
    if (strstr(node->var.name, "tmp") != NULL) {
      sprintf(nameBuf, ", %s", node->var.name);
      strcat(tmps, nameBuf);
    }
    node = node->next;
  }
  strcat(tmps, ";\n");
  printf("\n%s", tmps);
}

void printStatements(stack *s) {
  int counter = 1;
  stackNode *node = s->head;
  while (node) {
    if (strstr(node->expression, "if(") ==
        NULL) { // if "if" is not in the statement, print here
      printf("\tS%d:	%s%s\n", counter, node->var.name, node->expression);
    } else {
      char expBuf[1000]; // current expression
      sprintf(expBuf, "%s", node->expression);
      char *expTemp = strtok(expBuf, "\n");
      while (expTemp) {
        printf("\tS%d:	%s\n", counter, expTemp);
        counter++;
        expTemp = strtok(NULL, "\n");
      }
    }
    counter++;
    node = node->next;
  }
}

void printVars(stack *s) {
  printf("\n");
  stackNode *node = s->head;
  while (node) {
    char name[100];
    if (strstr(node->var.name, "tmp") == NULL) {
      printf("\tprintln(\"%s = %%d\\n\", %s);\n", node->var.name,
             node->var.name);
    }
    node = node->next;
  }
  printf("}\n");
}

void task3Main(char *usrVars, stack *s) {
  printf("main(){\n");
  // initialize variables
  initVars(s);
  // initialize temps
  initTmps(s);
  // print user variables
  if (usrVars) {
    char *stringBuf = strtok(usrVars, " ");
    while (stringBuf) {
      printf("\tprintf(\"%s=\");\n", stringBuf);
      printf("\tscanf(\"%%d\", &%s);\n", stringBuf);
      stringBuf = strtok(NULL, " ");
    }
    printf("\n");
    printStatements(s);
    printVars(s);
  }
}
