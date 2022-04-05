infix: infix.tab.o lex.yy.o
	gcc -o infix lex.yy.o infix.tab.o -lm

lex.yy.o: infix.l
	flex infix.l; gcc -c lex.yy.c

infix.tab.o: infix.y
	bison -d infix.y; gcc -c infix.tab.c

clean: 
	rm -f p2 infix.output *.o infix.tab.c lex.yy.c