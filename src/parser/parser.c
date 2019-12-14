#include "parser.h"
#include "../lib/bool.h"
#include "../lib/lib.h"
#include <stdlib.h>
#include <string.h>

//----------------------------------------------------------------------------------
// Declaracion de funciones estaticas.
//----------------------------------------------------------------------------------
static bool is_current_token(const Parser *const parser, const char *token);
static bool is_peek_token(const Parser *const parser, const char *token);
static bool expect_token(Parser *parser, const char *token);

//----------------------------------------------------------------------------------
// Declaracion de funciones estaticas.
//----------------------------------------------------------------------------------
Parser new_parser(Lexer *lexer) {
    Parser parser = {0};
    parser._lexer = lexer;
    // Inicializar token sin definicion
    parser._current_token = (Token) {._type=BEV_UNDEFINED, ._literal=NULL};
    parser._peek_token = (Token) {._type=BEV_UNDEFINED, ._literal=NULL};

    next_token_parser(&parser);
    next_token_parser(&parser);

    return parser;
}

void free_parser(Parser *parser) {
    if (parser != NULL && parser->_lexer != NULL) {
        free_lexer(parser->_lexer);
        parser->_lexer = NULL;
    }
}

void next_token_parser(Parser *parser) {
    if (parser->_current_token._type != BEV_UNDEFINED) 
        free_token(&parser->_current_token);

    parser->_current_token = parser->_peek_token;
    parser->_peek_token = next_token_lexer(&(*parser->_lexer));
}

Program program_parser(Parser *parser) {
    Program program = new_program(10);
    program._statements = NULL;

    Statement stmt = {0};
    while (parser->_current_token._type != BEV_EOF) {
        stmt = stmt_parser(parser);

        // Ocurrio algun error en la construccion del AST.
        if(stmt._type == TYPE_FAILURE) {
            free_program(&program);
            break;
        }
        else {
            if (stmt._ptr != NULL)
                add_stmt_program(&program, stmt._ptr, stmt._type);

            next_token_parser(parser);
        }
    }   

    return program;
}

Statement stmt_parser(Parser *parser) {
    Statement stmt = {0};

    if (parser->_current_token._type == BEV_LET) 
        return let_stmt_parser(parser);

    return stmt;
}

Statement let_stmt_parser(Parser *parser) {
    Statement stmt = (Statement) {0};
    stmt._type = TYPE_LET;
    ((LetStatement *)stmt._ptr)->_token = new_token(parser->_current_token._type, copy_string(parser->_current_token._literal));

    // Si el proximo token es diferente a un BEV_IDENT
    // genero un error (FAILURE).
    if (!expect_token(parser, BEV_IDENT)) {
        // Elimina la memoria reservada del token.
        free_token(&((LetStatement *)stmt._ptr)->_token);
        return (Statement) {._ptr=NULL, ._type=TYPE_FAILURE};;
    }

    // Inicializo el identificador.
    ((LetStatement *)stmt._ptr)->_name = new_identifier(&parser->_current_token, parser->_current_token._literal);

    // Si el token actual es diferente a un "punto y coma"
    // genero un error (FAILURE).
    if (!is_current_token(parser, BEV_SEMICOLON)) {
        // Elimina la memoria reservada del token.
        free_token(&((LetStatement *)stmt._ptr)->_token);
        // Elimina la memoria reservada de un identificador.
        free_identifier(&((LetStatement *)stmt._ptr)->_name);
        return (Statement) {._ptr=NULL, ._type=TYPE_FAILURE};
    }

    return stmt;
}

//----------------------------------------------------------------------------------
// Implementacion de funciones estaticas.
//----------------------------------------------------------------------------------
static bool is_current_token(const Parser *const parser, const char *token) {
    return (strcmp(parser->_current_token._type, token) == 0);
}

static bool is_peek_token(const Parser *const parser, const char *token) {
    return (strcmp(parser->_peek_token._type, token) == 0);
}

static bool expect_token(Parser *parser, const char *token) {
    if (is_peek_token(parser, token)) {
        next_token_parser(parser);
        return true;
    }

    return false;
}