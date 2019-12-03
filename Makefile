NAME := beverly
CC := gcc
CFLAGS := -Wall -pedantic -std=c99
LEXER_TEST := lexer_test
DEBUG := 1

ifdef DEBUG
	CFLAGS += -g
else
	CFLAGS += -O3
endif

main: dist obj obj/main.o obj/repl.o obj/token.o obj/lexer.o 
	$(CC) -o dist/$(NAME) obj/main.o obj/repl.o obj/token.o obj/lexer.o $(CFLAGS)

obj/main.o: main.c src/repl.h
	$(CC) -c main.c -o obj/main.o $(CFLAGS)

obj/lexer.o: src/lexer.c src/lexer.h src/bool.h src/token.h
	$(CC) -c src/lexer.c -o obj/lexer.o $(CFLAGS)  

obj/token.o: src/token.c src/token.h
	$(CC) -c src/token.c -o obj/token.o $(CFLAGS) 

obj/repl.o: src/repl.c src/repl.h src/bool.h 
	$(CC) -c src/repl.c -o obj/repl.o $(CFLAGS)  

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
lexer_test: obj obj/lexer_test.o obj/token.o obj/lexer.o
	$(CC) -o $(LEXER_TEST) obj/lexer_test.o obj/token.o obj/lexer.o $(CFLAGS)

obj/lexer_test.o: src/lexer_test.c
	$(CC) -c src/lexer_test.c -o obj/lexer_test.o $(CFLAGS)

.PHONY:clean cleanall
clean:
	del /s *.o
cleanall:
	del /s *.o
	del *.exe
	rmdir /s obj
	rmdir /s dist