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

static bool __test_returnStatement(void);

static bool __test_identifier(void);

static bool __test_integerLiteral(void);

static bool __test_prefixExpression(void);

//----------------------------------------------------------------------------------
// Funcion main.
//----------------------------------------------------------------------------------
int main(void) {
    assert(__test_letStatement());
    printf("__test_letStatement (COMPLETED).\n");

    assert(__test_returnStatement());
    printf("__test_returnStatement (COMPLETED).\n");

    assert(__test_identifier());
    printf("__test_identifier (COMPLETED).\n");

    assert(__test_integerLiteral());
    printf("__test_integerLiteral (COMPLETED).\n");

    return 0;
}

//----------------------------------------------------------------------------------
// Implementacion de funciones estaticas.
//----------------------------------------------------------------------------------
static bool __test_prefixExpression(void) {
    // Falta implementar
}

static bool __test_integerLiteral(void) {
    const char *input = "5";

    Lexer lexer = new_lexer(input);
    Parser parser = new_parser(&lexer);

    Program program = program_parser(&parser);
    if (checkParserErrors(&parser)) {
        delete_data(&program, &lexer, &parser);
        return false;
    }

    if (program._statements == NULL) {
        printf("Error, program is NULL\n");
        delete_data(&program, &lexer, &parser);
        return false;
    }

    if (program._len != 1) {
        printf("Error, program._len has been 1 statements. got=%d.\n", program._len);
        delete_data(&program, &lexer, &parser);
        return false;
    }

    ExpressionStatement *aux_exprStmt = ((ExpressionStatement *)program._statements[0]._ptr);
    IntegerLiteral *aux_iLiteral = ((IntegerLiteral *)aux_exprStmt->_expression._ptr);

    if (aux_iLiteral->_value != 5) {
        printf("Error, value not %d. got=%d\n", 5, aux_iLiteral->_value);
        delete_data(&program, &lexer, &parser);
        return false;
    }

    if (strcmp(aux_iLiteral->_token._literal, "5") != 0) {
        printf("Error, _literal not %s. got=%s\n", "5", aux_iLiteral->_token._literal);
        delete_data(&program, &lexer, &parser);
        return false;
    }

    delete_data(&program, &lexer, &parser);
    return true;
}

static bool __test_identifier(void) {
    const char *input = "foobar";

    Lexer lexer = new_lexer(input);
    Parser parser = new_parser(&lexer);

    Program program = program_parser(&parser);
    if (checkParserErrors(&parser)) {
        delete_data(&program, &lexer, &parser);
        return false;
    }

    if (program._statements == NULL) {
        printf("Error, program is NULL\n");
        delete_data(&program, &lexer, &parser);
        return false;
    }

    if (program._len != 1) {
        printf("Error, program._len has been 1 statements. got=%d.\n", program._len);
        delete_data(&program, &lexer, &parser);
        return false;
    }

    ExpressionStatement *aux_exprStmt = ((ExpressionStatement *)program._statements[0]._ptr);
    Identifier *aux_ident = ((Identifier *)aux_exprStmt->_expression._ptr);

    if (strcmp(aux_ident->_value, "foobar") != 0) {
        printf("Error, ident.value not %s. got=%s\n", "foobar", ((Identifier *)((ExpressionStatement *)program._statements[0]._ptr)->_expression._ptr)->_value);
        delete_data(&program, &lexer, &parser);
        return false;
    }

    delete_data(&program, &lexer, &parser);
    return true;
}

static bool __test_returnStatement(void) {
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
        printf("Error, program is NULL\n");
        delete_data(&program, &lexer, &parser);
        return false;
    }

    if (program._len != 3) {
        printf("Error, program._len has been 3 statements. got=%d.\n", program._len);
        delete_data(&program, &lexer, &parser);
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
        printf("Error, program is NULL\n");
        delete_data(&program, &lexer, &parser);
        return false;
    }

    if (program._len != 3) {
        printf("Error, program._len has been 3 statements. got=%d.\n", program._len);
        delete_data(&program, &lexer, &parser);
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
            delete_data(&program, &lexer, &parser);
            printf("Error, statement with index '%d' is not LET type.\n", k);
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
