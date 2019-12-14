#include "parser.h"
#include "../lib/bool.h";
#include <stdlib.h>

//----------------------------------------------------------------------------------
// Declaracion de funciones estaticas.
//----------------------------------------------------------------------------------
static bool is_current_token(const Parser *const parser, const Token *const token);
static bool is_peek_token(const Parser *const parser, const Token *const token);
static bool expect_token(Parser *parser, const Token *const token);

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
        free_lexer(&parser->_lexer);
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
        if(stmt._type == FAILURE) {
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
    LetStatement let_stmt = (LetStatement) {0};
    
    // Inicializo el token de la declaracion.
    let_stmt._token = new_token(parser->_current_token._type, parser->_current_token._literal);

    // Si el proximo token es diferente a un BEV_IDENT
    // genero un error (FAILURE).
    if (!expect_token(parser, BEV_IDENT)) 
        return (Statement) {._ptr=NULL, ._type=FAILURE};

    let_stmt._name = 

    return stmt;
}

//----------------------------------------------------------------------------------
// Implementacion de funciones estaticas.
//----------------------------------------------------------------------------------
static bool is_current_token(const Parser *const parser, const Token *const token) {
    return (parser->_current_token._type == token->_type);
}

static bool is_peek_token(const Parser *const parser, const Token *const token) {
    return (parser->_peek_token._type == token->_type);
}

static bool expect_token(Parser *parser, const Token *const token) {
    if (is_peek_token(parser, token)) {
        next_token_parser(parser);
        return true;
    }

    return false;
}