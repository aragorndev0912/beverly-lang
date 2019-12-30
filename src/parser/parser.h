#ifndef _PARSER_H
#define _PARSER_H

#include "../ast/ast.h"
#include "../lexer/lexer.h"
#include "../token/token.h"

//----------------------------------------------------------------------------------
// Declaracion de ParserError.
//----------------------------------------------------------------------------------
typedef struct ParserError {
    char **_errors;
    size_t _len;
    size_t _cap;

} ParserError;

ParserError new_parsererror(void);

void free_parsererror(ParserError *p_error);

void add_parsererror(ParserError *p_error, const char *error);


//----------------------------------------------------------------------------------
// Declaracion de Parser.
//----------------------------------------------------------------------------------
typedef struct Parser {
    Lexer *_lexer;
    Token _current_token;
    Token _peek_token;
    ParserError error;

} Parser;

Parser new_parser(Lexer *lexer);

void free_parser(Parser *parser);

void next_token_parser(Parser *parser);

Program program_parser(Parser *parser);

Statement stmt_parser(Parser *parser);

void peek_error_parser(Parser *parser, const char *token);

#endif //_PARSER_H