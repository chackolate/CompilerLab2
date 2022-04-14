#include "varNodeStack.h"
#include <ctype.h>
#include <stdbool.h>
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

void task1Main(stack *s) { printStack(s); }

char **findUsrVars(stack *s, int *size, char **varList) {

  stackNode *node = s->head;
  int i = 0;
  while (node) {
    varList[i] = (char *)malloc(sizeof(char) * 20);
    char nameBuf[100];
    // if (strstr(node->var.name, "tmp") == NULL) {
    sprintf(nameBuf, "%s", node->var.name);
    strcat(varList[i], nameBuf);
    // }
    i++;
    node = node->next;
  }
  *size = i;
  return varList;
}

// --------------------Task 2--------------------
char *readStack(stack *s) {
  char *stackAttributes = malloc(sizeof(char) * 500);
  stackNode *node = s->head;
  while (node) {
    char lineBuf[100];
    if (strstr(node->expression, "if(") == NULL) {
      sprintf(lineBuf, "%s%s", node->var.name, node->expression);
    } else {
      sprintf(lineBuf, "%s", node->expression);
    }
    strcat(stackAttributes, lineBuf);
    // sprintf(lineBuf, "\n");
    // strcat(stackAttributes, lineBuf);
    node = node->next;
  }
  return stackAttributes;
}

void divideStack(stack *s, char **instructions,
                 int *size) { // return 2D array of instructions
  char *stackRead = readStack(s);
  // printf("%s\n", stackRead);

  char *stackTok = strtok(stackRead, "\n");

  int i = 0;
  while (stackTok != NULL) {
    instructions[i] = malloc(20 * sizeof(char));
    instructions[i] = stackTok;
    // strcat(instructions[i], "\n");
    i++;
    stackTok = strtok(NULL, "\n");
  }
  *size = i;
}

int findFirstLine(char **instructions, int lineCnt, char *usrVar) {
  for (int i = 0; i < lineCnt; i++) {
    if (strstr(instructions[i], usrVar)) {
      // printf("%s first line %d\n", usrVar, i);
      return i;
    }
  }
}

int findLastLine(char **instructions, int lineCnt, char *usrVar) {
  int lastLine = 0;
  for (int i = 0; i < lineCnt; i++) {
    if (strstr(instructions[i], usrVar)) {
      if (i >= lastLine) {
        lastLine = i;
      }
    }
  }
  // printf("%s last line %d\n", usrVar, lastLine);
  return lastLine;
}

void fillNodes(regNode *vars, char **varsBuf, char **instructions, int lineCnt,
               int numVars) {
  for (int i = 0; i < numVars; i++) {
    strcpy(vars[i].name, varsBuf[i]);
    vars[i].firstPos = findFirstLine(instructions, lineCnt, varsBuf[i]);
    vars[i].lastPos = findLastLine(instructions, lineCnt, varsBuf[i]);
    printf("%s : %d->%d\n", vars[i].name, vars[i].firstPos, vars[i].lastPos);
  }
}

bool checkLive(regNode var1, regNode var2, int *bound1, int *bound2) {
  bool live = false;
  int firstLine, lastLine = 0;

  if ((var1.firstPos >= var2.firstPos) && (var1.firstPos < var2.lastPos)) {
    // var1 starts after/same line as var2 and before var2 ends
    live = true;
    firstLine = var1.firstPos;
    if (var1.lastPos > var2.lastPos) { // var1 ends after var2
      lastLine = var2.lastPos;
    } else if (var2.lastPos >= var1.lastPos) { // var2 ends after var1
      lastLine = var1.lastPos;
    }

  } else if ((var1.firstPos <= var2.firstPos) &&
             (var1.lastPos > var2.firstPos)) {
    // var1 starts before/same line as var2 and ends after var2 starts
    live = true;
    firstLine = var2.firstPos;
    if (var1.lastPos > var2.lastPos) {
      lastLine = var2.lastPos;
    } else if (var2.lastPos >= var1.lastPos) {
      lastLine = var1.lastPos;
    }
  }

  if (live) {
    // printf("%s and %s live from %d to %d\n", var1.name, var2.name, firstLine,
    //  lastLine);
  }
  *bound1 = firstLine;
  *bound2 = lastLine;

  return live;
}

// return edge objects for one variable
void createEdgeArray(regNode var, regNode *vars, int numVars,
                     regNodeEdge *edges, int *numEdges) {
  int counter = 0;
  for (int i = 0; i < numVars; i++) {
    int bound1, bound2;
    if (strcmp(var.name, vars[i].name)) {
      if (checkLive(var, vars[i], &bound1, &bound2)) {
        edges[counter].src = var;
        edges[counter].dst = vars[i];
        counter++;
      }
    }
  }
  *numEdges = counter;
}

graphNode *createGraphNode(regNode *vars, regNode *src, int numVars,
                           regNodeEdge *edges, int *numEdges) {
  createEdgeArray(*src, vars, numVars, edges, numEdges);
  graphNode *node = (graphNode *)malloc(sizeof(graphNode));
  node->src = (char *)malloc(10 * sizeof(char));
  strcpy(node->src, src->name);
  node->dsts = (char **)malloc(*numEdges * sizeof(char *));
  for (int i = 0; i < *numEdges; i++) {
    node->dsts[i] = (char *)malloc(10 * sizeof(char));
    strcpy(node->dsts[i], edges[i].dst.name);
  }
  node->edges = edges;
  node->numEdges = *numEdges;
}

void printGraphNode(graphNode *node) {
  for (int i = 0; i < node->numEdges; i++) {
    printf("(%s->%s)", node->src, node->dsts[i]);
  }
  printf("\n");
}

// regNodeGraph *createGraph(regNodeGraph *graph, regNodeEdge **edges,
//                           int edgeCnts, regNode *vars, int numVars) {

//   for (int i = 0; i < graph->numEdges; i++) {
//   }
//   graph->edges = edges;
//   graph->head = vars;
//   graph->numVars = numVars;
//   graph->numEdges = edgeCnts;
//   return graph;
// }

nodeGraph *createGraph(graphNode **nodes, int numNodes) {
  nodeGraph *graph = (nodeGraph *)malloc(sizeof(nodeGraph));
  graph->nodes = nodes;
  graph->numNodes = numNodes;
}

void printGraph(nodeGraph *graph,
                int numVars) { // print graph by looping through total #
                               // of edges and counts per edge group
  for (int i = 0; i < graph->numNodes; i++) {
    for (int j = 0; j < graph->nodes[i]->numEdges; j++) {
      printf("(%s->%s)", graph->nodes[i]->src, graph->nodes[i]->dsts[j]);
    }
    printf("\n");
  }
}

void assignRegs(
    regNode *vars, int numVars, int numLines,
    char **instructions) { // greedy algorithm - step through program &
                           // assign nodes to registers in order
  char *currentLine = (char *)malloc(100 * sizeof(char));
  registerFile *regFile = (registerFile *)malloc(4 * sizeof(registerFile));
  for (int i = 0; i < 4; i++) {
    regFile[i].occupied = 0;
  }
  int lastReg = 0;
  regNode *currentVars = (regNode *)malloc(numVars * sizeof(regNode));

  for (int i = 0; i < numVars; i++) {
    memcpy(&currentVars[i], &vars[i], sizeof(regNode *));
  }
  // step 0
  for (int i = 0; i < numLines; i++) {
    for (int j = 0; j < numVars; j++) {
      if (currentVars[j].firstPos >=
          i) { // encountered first line, allocate register
        if (!regFile[lastReg].occupied) {
          regFile[lastReg].reg = currentVars[j];
          printf("reg%d=%s\n", lastReg, currentVars[j].name);
          regFile[lastReg].occupied = 1;
          lastReg++;
          if (lastReg > 3) {
            lastReg = 0;
          }
        }
      } else if (currentVars[j].lastPos <=
                 i) { // encountered last line, clear register
        for (int x = 0; x < 4; x++) {
          if (!strcmp(currentVars[j].name, regFile[x].reg.name)) {
            regFile[x].occupied = 0;
            printf("reg%d empty\n", x);
            lastReg = x;
          }
        }
      }
    }
  }
}

void task2Main(stack *s) {

  // get user variables list
  int numVars = 0;
  char **varsBuf = (char **)malloc(20 * sizeof(char *));
  varsBuf = findUsrVars(s, &numVars, varsBuf);
  for (int i = 0; i < numVars; i++) {
    printf("%s", varsBuf[i]);
  }
  // printf("vars: %s\n", varsBuf);
  printf("\nnumber of vars: %d\n", numVars);

  // split stack into 2d array of strings
  char **instructions = (char **)malloc(20 * sizeof(char *));
  int lineCnt;
  divideStack(s, instructions, &lineCnt);
  printf("%d lines of instructions\n", lineCnt);
  for (int i = 0; i < lineCnt; i++) {
    printf("%s\n", instructions[i]);
  }

  // create variable nodes with info on their names and first/last lines
  regNode *vars = malloc(numVars * sizeof(*vars));
  fillNodes(vars, varsBuf, instructions, lineCnt, numVars);
  int bound1, bound2;

  // analyze liveness and create edge objects
  for (int i = 0; i < numVars; i++) {
  }

  regNodeEdge **edges = malloc(numVars * sizeof(**edges));
  int edgeCnts[numVars]; // edge counters for each variable
  for (int i = 0; i < numVars; i++) {
    edges[i] = (regNodeEdge *)malloc(10 * sizeof(regNodeEdge));
    // createEdgeArray(vars[i], vars, numVars, edges[i], &edgeCnts[i]);
  }
  // for (int i = 0; i < numVars;
  //      i++) { // top loop: # of edge arrays = # of variables
  //   for (int j = 0; j < edgeCnts[i];
  //        j++) { // bottom loop: each edge array has its own counter
  //     printf("(%s->%s) ", edges[i][j].src.name, edges[i][j].dst.name);
  //   }
  //   printf("\n");
  // }
  printf("analyzed edges\n");
  graphNode **node = (graphNode **)malloc(
      numVars * sizeof(graphNode *)); // array of graph nodes
  // createGraphNode(node, vars, vars[0], numVars, edges[0], &edgeCnts[0]);

  for (int i = 0; i < numVars;
       i++) { // outer loop: go through every variable and create a graph node
    node[i] = createGraphNode(vars, &vars[i], numVars, edges[i], &edgeCnts[i]);
  }

  for (int i = 0; i < numVars; i++) {
    printGraphNode(node[i]);
  }
  printf("created graph nodes\n");

  // create graph out of nodes
  nodeGraph *graph = createGraph(node, numVars);
  printGraph(graph, numVars);

  // // read graph to assign registers
  assignRegs(vars, numVars, lineCnt, instructions);
}

// --------------Task 3-----------------
void printVars(stack *s) {
  char output[1000];
  sprintf(output, "\tint");
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
      strcat(output, nameBuf);
    }
    node = node->next;
  }
  while (node) { // consecutive elements print with comma
    if (strstr(node->var.name, "tmp") == NULL) {
      // printf("condition 2\n");
      sprintf(nameBuf, ", %s", node->var.name);
      strcat(output, nameBuf);
    }
    node = node->next;
  }
  strcat(output, ";\n");
  printf("\n%s", output);
}

void printTmps(stack *s) {
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
        NULL) { // if tmp is found in a name, add to list
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

void printUsrVars(stack *s, char *usrVars) {
  if (usrVars) {
    char *stringBuf = strtok(usrVars, " ");
    while (stringBuf) {
      printf("\tprintf(\"%s=\");\n", stringBuf);
      printf("\tscanf(\"%%d\", &%s);\n", stringBuf);
      stringBuf = strtok(NULL, " ");
    }
    printf("\n");
  }
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

void reprintVars(stack *s) {
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
  printVars(s);
  // initialize temps
  printTmps(s);
  // print user variables
  printUsrVars(s, usrVars);
  printStatements(s);
  reprintVars(s);
}
