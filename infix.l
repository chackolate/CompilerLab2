%{
	#include <stdlib.h>
	#include <string.h>
	#include "infix.tab.h"

	#define VERBOSE


	extern int lineNum;
	extern char *yytext;

%}


%%
[ \t]+ {}

[\n] {
	lineNum++;
	return '\n';
}

"quit()" {
	#ifdef VERBOSE
	printf("'quit()':%d\n", lineNum);
	#endif
	return QUIT;
}

"=" {
	#ifdef VERBOSE
	printf("'=':%d\n", lineNum);
	#endif
	return '=';
}

"+" {
	#ifdef VERBOSE
	printf("'+':%d\n", lineNum);
	#endif
	return '+';
}

"-" {
	#ifdef VERBOSE
	printf("'-':%d\n", lineNum);
	#endif
	return '-';
}

"*" {
	#ifdef VERBOSE
	printf("'*':%d\n", lineNum);
	#endif
	return '*';
}

"/" {
	#ifdef VERBOSE
	printf("'/':%d\n", lineNum);
	#endif
	return '/';
}

"**" {
	#ifdef VERBOSE
	printf("'EXP':%d\n", lineNum);
	#endif
	return EXP;
}

"(" {
	#ifdef VERBOSE
	printf("'(':%d\n", lineNum);
	#endif
	return '(';
}

")" {
	#ifdef VERBOSE
	printf("')':%d\n", lineNum);
	#endif
	return ')';
}

"!" {
	#ifdef VERBOSE
	printf("'!':%d\n", lineNum);
	#endif
	return '!';
}

"?"{
	#ifdef VERBOSE
	printf("'?':%d\n",lineNum);
	#endif
	return '?';
}

[0-9]+ {
	#ifdef VERBOSE
	printf("%s:%d\n", yytext, lineNum);
	#endif
	yylval.d = atoi(yytext);
	return NUM;
}

[a-z][a-zA-Z0-9]* {
	#ifdef VERBOSE
	printf("'%s':%d\n", yytext, lineNum);
	#endif
	return TXT;
}

%%

int yywrap(){
	return 1;
}

