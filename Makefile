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

obj/main.o: main.c src/repl/repl.h
	$(C) -c main.c -o obj/main.o $(CCFLAGS)

obj/lexer.o: src/lexer/lexer.c src/lexer/lexer.h src/lib/bool.h src/token/token.h
	$(C) -c src/lexer/lexer.c -o obj/lexer.o $(CCFLAGS)  

obj/token.o: src/token/token.c src/token/token.h
	$(C) -c src/token/token.c -o obj/token.o $(CCFLAGS) 

obj/repl.o: src/repl/repl.c src/repl/repl.h src/lib/bool.h 
	$(C) -c src/repl/repl.c -o obj/repl.o $(CCFLAGS)  

obj/ast.o: src/ast/ast.c src/ast/ast.h src/token/token.h
	$(C) -c src/ast/ast.c -o obj/ast.o $(CCFLAGS)

obj/parser.o: src/parser/parser.c src/parser/parser.h src/lexer/lexer.h src/token/token.h
	$(C) -c src/parser/parser.c -o obj/parser.o $(CCFLAGS)

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

obj/lexer_test.o: src/lexer/lexer_test.c
	$(C) -c src/lexer/lexer_test.c -o obj/lexer_test.o $(CCFLAGS)

#------------------------------------------------------------------------
# Test Parser.
#------------------------------------------------------------------------
parser_test: obj obj/parser_test.o obj/parser.o obj/lexer.o obj/ast.o obj/token.o
	$(C) -o parser_test obj/parser_test.o obj/parser.o obj/lexer.o obj/ast.o obj/token.o $(CCFLAGS)

obj/parser_test.o: src/parser/parser_test.c 
	$(C) -c src/parser/parser_test.c -o obj/parser_test.o $(CCFLAGS)


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