infix: infix.tab.o lex.yy.o stack.o
	gcc -o infix lex.yy.o infix.tab.o stack.o -lm

lex.yy.o: infix.l
	flex infix.l; gcc -c lex.yy.c

infix.tab.o: infix.y
	bison -d infix.y; gcc -c infix.tab.c

stack.o: stack.c
	gcc -c stack.c

clean: 
	rm -f p2 infix.output *.o infix.tab.c lex.yy.c