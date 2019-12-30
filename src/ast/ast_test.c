#include <stdio.h>
#include <assert.h>
#include "../ast/ast.h"
#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "../lib/bool.h"

//----------------------------------------------------------------------------------
// Firmas de funciones estaticas.
//----------------------------------------------------------------------------------
static void delete_data(Program *program, Lexer *lexer, Parser *parser);

static bool __test_string(void);

//----------------------------------------------------------------------------------
// Funcion main.
//----------------------------------------------------------------------------------
int main(void) {
    assert(__test_string());
    printf("__test_string (COMPLETED).\n");

    return 0;
}

//----------------------------------------------------------------------------------
// Implementacion de funciones estaticas.
//----------------------------------------------------------------------------------
static bool __test_string(void) {
    const char *input = "let myVar = anotherVar;";
    
    Lexer lexer = new_lexer(input);
    Parser parser = new_parser(&lexer);

    Program program = program_parser(&parser);

    printf("|%s|\n", string_program(&program));

    delete_data(&program, &lexer, &parser);


    return true;
}

static void delete_data(Program *program, Lexer *lexer, Parser *parser) {
    free_program(program);
    free_lexer(lexer);
    free_parser(parser);
}