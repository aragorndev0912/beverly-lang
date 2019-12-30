#include "parser.h"
#include "../lib/bool.h"
#include "../lib/lib.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//----------------------------------------------------------------------------------
// Declaracion de constantes.
//----------------------------------------------------------------------------------

static const size_t PARSER_ERROR_SIZE = 10;

//----------------------------------------------------------------------------------
// Declaracion de funciones estaticas.
//----------------------------------------------------------------------------------
static bool is_current_token(const Parser *const parser, const char *token);
static bool is_peek_token(const Parser *const parser, const char *token);
static bool expect_token(Parser *parser, const char *token);


static Statement letStmt_parser(Parser *parser);
static Statement returnStmt_parser(Parser *parser);

//----------------------------------------------------------------------------------
// Implementacion de funciones ParserError.
//----------------------------------------------------------------------------------
ParserError new_parsererror(void) {
    ParserError p_error;
    p_error._errors = NULL;
    p_error._len = 0;
    p_error._cap = 0;

    return p_error;
}

void free_parsererror(ParserError *p_error) {
    if (p_error->_errors != NULL) {
        for (int k=0; k < p_error->_len; k++) {
            free(p_error->_errors[k]);
            p_error->_errors[k] = NULL;
        }

        free(p_error->_errors);
        p_error->_errors = NULL;
    }
}

void add_parsererror(ParserError *p_error, const char *error) {
    if (p_error->_errors == NULL) {
        p_error->_cap = PARSER_ERROR_SIZE;
        p_error->_errors = (char **) malloc(sizeof(char *) * p_error->_cap);
        if (p_error->_errors == NULL) {
            // I need to implement it.
        }
        p_error->_errors[p_error->_len++] = copy_string(error);
    }
    else {
        if (p_error->_len >= p_error->_cap) {
            p_error->_cap += PARSER_ERROR_SIZE; 
            char **_tmp = (char **) realloc(p_error->_errors, p_error->_cap);
            if (_tmp == NULL) {
                // Falta implementacion.
            }
            p_error->_errors = _tmp;
            _tmp = NULL;
        }

        p_error->_errors[p_error->_len++] = copy_string(error);
    }
}

//----------------------------------------------------------------------------------
// Implementacion de funciones Parser.
//----------------------------------------------------------------------------------
Parser new_parser(Lexer *lexer) {
    Parser parser = {0};
    parser._lexer = lexer;
    // Inicializar token sin definicion
    parser._current_token = (Token) {._type=BEV_UNDEFINED, ._literal=NULL};
    parser._peek_token = (Token) {._type=BEV_UNDEFINED, ._literal=NULL};

    next_token_parser(&parser);
    next_token_parser(&parser);

    parser.error = new_parsererror();

    return parser;
}

void free_parser(Parser *parser) {
    if (parser != NULL) {
        if (parser->_lexer != NULL) {
            // free_lexer(parser->_lexer);
            parser->_lexer = NULL;
        }

        if (parser->error._errors != NULL) {
            for (int k=0; k < parser->error._len; k++) {
                free(parser->error._errors[k]);
                parser->error._errors[k] = NULL;
            }
        }   
    }
}

void next_token_parser(Parser *parser) {
    if (parser->_current_token._type != BEV_UNDEFINED) 
        free_token(&parser->_current_token);

    parser->_current_token = parser->_peek_token;
    parser->_peek_token = next_token_lexer(&(*parser->_lexer));
}

Program program_parser(Parser *parser) {
    Program program = new_program();
    program._statements = NULL;

    Statement stmt = (Statement) {0};
    while (parser->_current_token._type != BEV_EOF) {
        stmt = stmt_parser(parser);
        // Ocurrio algun error en la construccion del AST.
        if (stmt._ptr != NULL)
            add_stmt_program(&program, stmt._ptr, stmt._type);

        next_token_parser(parser);
    }   

    free_token(&parser->_current_token);
    free_token(&parser->_peek_token);

    return program;
}

Statement stmt_parser(Parser *parser) {
    Statement stmt = (Statement) {._ptr=NULL, ._type=TYPE_FAILURE};
    if (parser->_current_token._type == BEV_LET) 
        return letStmt_parser(parser);
    else if (parser->_current_token._type == BEV_RETURN)
        return  returnStmt_parser(parser);

    return stmt;
}

static Statement returnStmt_parser(Parser *parser) {
    Statement stmt = (Statement) {0};
    stmt._type = TYPE_RETURN;
    
    stmt._ptr = (ReturnStatement *) malloc(sizeof(ReturnStatement));
    if (stmt._ptr == NULL) {
        // I need to implement it.
    }
    ((ReturnStatement *)stmt._ptr)->_token = new_token(
        parser->_current_token._type, 
        copy_string(parser->_current_token._literal)
    );
    ((ReturnStatement *)stmt._ptr)->__string = NULL;
    
    // Inicializo la Expresion con valores nulos.
    ((ReturnStatement *)stmt._ptr)->_value = (Expression) {0};

    next_token_parser(parser);

    while (!is_current_token(parser, BEV_SEMICOLON)) 
        next_token_parser(parser);

    return stmt;
}

static Statement letStmt_parser(Parser *parser) {
    Statement stmt = (Statement) {0};
    stmt._type = TYPE_LET;

    // Reservo memoria para una estructura de tipo LetStatement.
    stmt._ptr = (LetStatement *) malloc(sizeof(LetStatement));
    if (stmt._ptr == NULL) {
        // I need to implement it.
    }

    ((LetStatement *)stmt._ptr)->_token = new_token(parser->_current_token._type, copy_string(parser->_current_token._literal));
    ((LetStatement *)stmt._ptr)->__string = NULL;

    // Si el proximo token es diferente a un BEV_IDENT
    // genero un error (FAILURE).
    if (!expect_token(parser, BEV_IDENT)) {
        // Elimina la memoria reservada del token.
        free_token(&((LetStatement *)stmt._ptr)->_token);
        return (Statement) {._ptr=NULL, ._type=TYPE_FAILURE};;
    }

    // Inicializo el identificador.
    ((LetStatement *)stmt._ptr)->_name = new_identifier(&parser->_current_token, parser->_current_token._literal);

    // Por ahora no se examina las expresiones.
    while (!is_current_token(parser, BEV_SEMICOLON)) 
        next_token_parser(parser);
    
    return stmt;
}

void peek_error_parser(Parser *parser, const char *token) {
    char msg[200];
    sprintf(msg, "expected next token to be %s, got '%s' instead\n", token, parser->_peek_token._literal);

    add_parsererror(&parser->error, msg);
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

    peek_error_parser(parser, token);
    return false;
}