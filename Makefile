NAME=beverly
CC=gcc
CFLAGS=-Wall -g
CVERSION=c11
LEXER_TEST=lexer

main: main.o repl.o token.o lexer.o 
	$(CC) $(CFLAGS) -o $(NAME) main.o repl.o token.o lexer.o  -std=$(CVERSION)

main.o: main.c src/repl.h
	$(CC) $(CFLAGS) -c main.c -std=$(CVERSION)

lexer.o: src/lexer.c src/lexer.h src/bool.h src/token.h
	$(CC) $(CFLAGS) -c src/lexer.c -std=$(CVERSION)

token.o: src/token.c src/token.h
	$(CC) $(CFLAGS) -c src/token.c -std=$(CVERSION)

repl.o: src/repl.c src/repl.h src/bool.h 
	$(CC) $(CFLAGS) -c src/repl.c 

#************************************************************************
#************************************************************************
#************************************************************************

lexer_test: lexer_test.o token.o lexer.o
	$(CC) $(CFLAGS) -o $(LEXER_TEST) lexer_test.o token.o lexer.o -std=$(CVERSION)

lexer_test.o: src/lexer_test.c
	$(CC) $(CFLAGS) -c src/lexer_test.c -std=$(CVERSION)

.PHONY:clean all
clean:
	del /s *.o
all:
	del /s *.o
	del *.exe