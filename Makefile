NAME=beverly
CC=gcc
CFLAGS=-Wall -g
CVERSION=c99

main: main.o token.o lexer.o
	$(CC) $(CFLAGS) -o $(NAME) main.o lexer.o token.o -std=$(CVERSION)

main.o: main.c src/lexer.h
	$(CC) $(CFLAGS) -c main.c -std=$(CVERSION)

lexer.o: src/lexer.c src/lexer.h src/bool.h src/token.h
	$(CC) $(CFLAGS) -c src/lexer.c -std=$(CVERSION)

token.o: src/token.c src/token.h
	$(CC) $(CFLAGS) -c src/token.c -std=$(CVERSION)

.PHONY:clean all
clean:
	del /s *.o
all:
	del /s *.o
	del *.exe