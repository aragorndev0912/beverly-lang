#ifndef _PARSER_H
#define _PARSER_H

#include "../ast/ast.h"
#include "../lexer/lexer.h"
#include "../token/token.h"

typedef struct Parser {
    Lexer *_lexer;
    Token _current_token;
    Token _peek_token;

} Parser;

Parser new_parser(Lexer *lexer);

void free_parser(Parser *parser);

void next_token_parser(Parser *parser);

Program program_parser(Parser *parser);

Statement stmt_parser(Parser *parser);

Statement letstmt_parser(Parser *parser);

#endif //_PARSER_H