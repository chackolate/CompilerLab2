%{

	#include <stdio.h>
	#include <ctype.h>
	#include <string.h>
	#include <math.h>

	#define VERBOSE
	int lineNum = 1;
	int yylex();

	void yyerror(char *ps, ...){

	}

	
%}

%code {

	struct var{
		char name[100];
		int val;
		struct var *next;
	} var;

	struct var *head;

	struct var* assignVar(char *name, int val);
	struct var* findVar(char *name, int val, struct var *var);
	struct var* newVar(char* name, int val);
}

%union{
	char name[20];
	int d;
	struct var *nPtr;
}

%token <d> NUM
%token <name> TXT
%token QUIT
%right '='
%token '(' ')'
%left '+' '-'
%left '*' '/'
%right EXP
%right '!'
%type <d> expression factor term
%right '\n'
%start infix

%%

infix : expression '\n' {
					printf("=%d\n",$1);
				}
				| infix expression '\n'{
					printf("=%d\n",$2);
				}
				| infix '\n'{

				};
expression : expression '+' factor {
							$$ = $1 + $3;
						}
						| expression '-' factor {
							$$ = $1 - $3;
						}
						| factor {
							$$ = $1;
						};
factor : factor '*' term {
					$$ = $1 * $3;
				}
				| factor '/' term {
					$$ = $1 / $3;
				}
				| factor EXP term {
					$$ = pow($1,$3);
				}
				| term {
					$$ = $1;
				};
term : NUM{
				$$ = $1;
			}
			| '(' expression ')' {
				$$ = $2;
			}
			| TXT {
				struct var *node = findVar($1,0,head);
				$$ = node->val;
			}
			| '!' term {
				$$ = ($2 == 0) ? 1 : 0;
			}
			| TXT '=' expression {
				struct var *node = assignVar($1,$3);
				$$ = $3;
			};

%%

struct var* assignVar(char *name, int val){
	struct var *variable = findVar(name,val,head);
	variable->val = val;
	return variable;
}

struct var* findVar(char *name, int val, struct var *variable){
	if(strcmp(name, variable->name) == 0){
		return variable;
	}
	else if(variable->next == NULL){
		variable -> next = newVar(name,val);
		return variable -> next;
	}
	else{
		return findVar(name,val,variable->next);
	}
}

struct var* newVar(char* name, int val){
	struct var* var = (struct var*)malloc(sizeof(struct var));
	sscanf(name, "%s", var->name);
	var->val = val;
	var->next = NULL;
	return var;
}

void freeVars(){
	struct var *node = head->next;
	while(node != NULL){
		struct var *tmp = node->next;
		free(node);
		node = tmp;
	}
	return;
}

int main(){
	head = (struct var*)malloc(sizeof(struct var));
	yyparse();
	freeVars();
	return 0;
}