#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../ast/ast.h"
#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "../lib/bool.h"

//----------------------------------------------------------------------------------
// Firmas de funciones estaticas.
//----------------------------------------------------------------------------------
static void delete_data(Program *program, Lexer *lexer, Parser *parser);

static bool let_statement(const LetStatement *const let_stmt, const char *name);

static bool checkParserErrors(const Parser *const parser);

static bool __test_letStatement(void);

static bool _test_returnStatement(void);

//----------------------------------------------------------------------------------
// Funcion main.
//----------------------------------------------------------------------------------
int main(void) {
    assert(__test_letStatement());
    printf("__test_letStatement (COMPLETED).\n");

    assert(_test_returnStatement());
    printf("_test_returnStatement (COMPLETED).\n");

    return 0;
}

//----------------------------------------------------------------------------------
// Implementacion de funciones estaticas.
//----------------------------------------------------------------------------------

static bool _test_returnStatement(void) {
    const char *input = 
    "return 5;\n\
    return 10;\n\
    return 956345;";

    Lexer lexer = new_lexer(input);
    Parser parser = new_parser(&lexer);

    Program program = program_parser(&parser);
    if (checkParserErrors(&parser)) {
        delete_data(&program, &lexer, &parser);
        return false;
    }

    if (program._statements == NULL) {
        delete_data(&program, &lexer, &parser);
        printf("Error, program is NULL\n");
        return false;
    }

    if (program._len != 3) {
        delete_data(&program, &lexer, &parser);
        printf("Error, program._len has been 3 statements. got=%d.\n", program._len);
        return false;
    }

    for (int k=0; k < 3; k++) {
        if (program._statements[k]._type != TYPE_RETURN) {
            printf("Error, statement with index '%d' is not LET type.\n", k);
            delete_data(&program, &lexer, &parser);
            return false;
        }

        if (strcmp(((ReturnStatement *)program._statements[k]._ptr)->_token._literal, "return") != 0) {
            printf("returnStatemt._literal not 'return', got='%s'", ((ReturnStatement *)program._statements[k]._ptr)->_token._literal);
            delete_data(&program, &lexer, &parser);
            return false;
        }
    }


    delete_data(&program, &lexer, &parser);
    return true;
}

static bool __test_letStatement(void) {
    const char *input = 
    "let x = 5;\n\
    let y = 10;\n\
    let foobar = 838383;";

    Lexer lexer = new_lexer(input);
    Parser parser = new_parser(&lexer);
    
    Program program = program_parser(&parser);
    if (checkParserErrors(&parser)) {
        delete_data(&program, &lexer, &parser);
        return false;
    }

    if (program._statements == NULL) {
        delete_data(&program, &lexer, &parser);
        printf("Error, program is NULL\n");
        return false;
    }

    if (program._len != 3) {
        delete_data(&program, &lexer, &parser);
        printf("Error, program._len has been 3 statements. got=%d.\n", program._len);
        return false;
    }

    const char *tests[3] = {"x", "y", "foobar"};

    for (int k=0; k < 3; k++) {
        Statement stmt = program._statements[k];

        if (stmt._type == TYPE_LET) {
            if(!let_statement(((LetStatement *)stmt._ptr), tests[k])) {
                delete_data(&program, &lexer, &parser);
                return false;
            }
        }
        else {
            printf("Error, statement with index '%d' is not LET type.\n", k);
            delete_data(&program, &lexer, &parser);
            return false;
        }
    }

    delete_data(&program, &lexer, &parser);
    return true;
}

static void delete_data(Program *program, Lexer *lexer, Parser *parser) {
    free_program(program);
    free_lexer(lexer);
    free_parser(parser);
}

static bool let_statement(const LetStatement *const let_stmt, const char *name) {
    if (strcmp(let_stmt->_token._literal, "let") != 0) {
        printf("_literal is not 'let'. got=%s.\n", let_stmt->_token._literal);
        return false;
    }

    if (strcmp(let_stmt->_name._value, name) != 0) {
        printf("_value is not '%s'. got=%s.\n", name, let_stmt->_name._value);
        return false;
    }

    if (strcmp(let_stmt->_name._token._literal, name) != 0) {
        printf("_literal is not '%s'. got=%s.\n", name, let_stmt->_name._token._literal);
        return false;
    }

    return true;
}

static bool checkParserErrors(const Parser *const parser) {
    if (parser->error._errors == NULL) 
        return false;

    printf("parser has %d errors.\n", parser->error._len);
    for (int k=0; k < parser->error._len; k++) 
        printf(parser->error._errors[k]);

    return true;   
}