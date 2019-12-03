NAME := beverly
C := gcc
CCFLAGS := -Wall -pedantic -std=c99
LEXER_TEST := lexer_test
DEBUG := 1

ifdef DEBUG
	CCFLAGS += -g
else
	CCFLAGS += -O3
endif

main: dist obj obj/main.o obj/repl.o obj/token.o obj/lexer.o 
	$(C) -o dist/$(NAME) obj/main.o obj/repl.o obj/token.o obj/lexer.o $(CCFLAGS)

obj/main.o: main.c src/repl.h
	$(C) -c main.c -o obj/main.o $(CCFLAGS)

obj/lexer.o: src/lexer.c src/lexer.h src/bool.h src/token.h
	$(C) -c src/lexer.c -o obj/lexer.o $(CCFLAGS)  

obj/token.o: src/token.c src/token.h
	$(C) -c src/token.c -o obj/token.o $(CCFLAGS) 

obj/repl.o: src/repl.c src/repl.h src/bool.h 
	$(C) -c src/repl.c -o obj/repl.o $(CCFLAGS)  

obj/ast.o: src/ast.c src/ast.h src/token.h
	$(C) -c src/ast.c -o obj/ast.o $(CCFLAGS)

obj/parser.o: src/parser.c src/parser.h src/lexer.h src/token.h
	$(C) -c src/parser.c -o obj/parser.o $(CCFLAGS)

#------------------------------------------------------------------------
# Create folder dist and obj.
#------------------------------------------------------------------------
dist:
	mkdir dist

obj:
	mkdir obj

#------------------------------------------------------------------------
# Test Lexer.
#------------------------------------------------------------------------
lexer_test: obj obj/lexer_test.o obj/token.o obj/lexer.o
	$(C) -o $(LEXER_TEST) obj/lexer_test.o obj/token.o obj/lexer.o $(CCFLAGS)

obj/lexer_test.o: src/lexer_test.c
	$(C) -c src/lexer_test.c -o obj/lexer_test.o $(CCFLAGS)

#------------------------------------------------------------------------
# Test Parser.
#------------------------------------------------------------------------
parser_test: obj obj/parser_test.o obj/parser.o obj/lexer.o obj/ast.o obj/token.o
	$(C) -o parser_test obj/parser_test.o obj/parser.o obj/lexer.o obj/ast.o obj/token.o $(CCFLAGS)

obj/parser_test.o: src/parser_test.c 
	$(C) -c src/parser_test.c -o obj/parser_test.o $(CCFLAGS)


#------------------------------------------------------------------------
# PHONY.
#------------------------------------------------------------------------
.PHONY:clean cleanall
clean:
	del /s *.o
cleanall:
	del /s *.o
	del *.exe
	rmdir /s obj
	rmdir /s dist