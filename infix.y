%{

	#include <stdio.h>
	#include <ctype.h>
	#include <string.h>
	#include <math.h>
	#include "stack.h"

	#define VERBOSE
	int lineNum = 1;
	int yylex();

	void yyerror(char *ps, ...){
		printf("%s\n",ps);
	}

	extern FILE *yyin;

	stackNode *head;
	stack *s;

	int *counter;
	
%}

%code {

}

%union{
	char name[20];
	int d;
	stackNode *nPointer;
	stack nStack;
}

%token <d> NUM
%token <name> TXT
%token QUIT
%right '='
%token '(' ')'
%left '+' '-' '*' '/'
%right EXP
%right '!' '?'
%type <nPointer> expression factor exponent term
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

				}
				;
expression : expression '+' factor {
							$$ = $1 + $3;
						}
						| expression '-' factor {
							$$ = $1 - $3;
						}
						| factor {
							$$ = $1;
						}
						;
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
				}
				;
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
			}
			;

%%

int main(){
	head = (struct var*)malloc(sizeof(struct var));
	yyparse();
	freeVars();
	return 0;
}