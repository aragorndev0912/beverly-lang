NAME := beverly
CC := gcc
CFLAGS := -Wall -g -pedantic
CVERSION := c11
LEXER_TEST := lexer

main: dist obj obj/main.o obj/repl.o obj/token.o obj/lexer.o 
	$(CC) $(CFLAGS) -o dist/$(NAME) obj/main.o obj/repl.o obj/token.o obj/lexer.o  -std=$(CVERSION)

obj/main.o: main.c src/repl.h
	$(CC) $(CFLAGS) -c main.c -o obj/main.o -std=$(CVERSION)

obj/lexer.o: src/lexer.c src/lexer.h src/bool.h src/token.h
	$(CC) $(CFLAGS) -c src/lexer.c -o obj/lexer.o -std=$(CVERSION)

obj/token.o: src/token.c src/token.h
	$(CC) $(CFLAGS) -c src/token.c -o obj/token.o -std=$(CVERSION)

obj/repl.o: src/repl.c src/repl.h src/bool.h 
	$(CC) $(CFLAGS) -c src/repl.c -o obj/repl.o -std=$(CVERSION)

#************************************************************************
#************************************************************************
#************************************************************************
dist:
	mkdir dist

obj:
	mkdir obj
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
	rmdir /s dist
	rmdir /s obj