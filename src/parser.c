#include "parser.h"
#include <stdlib.h>

Parser new_parser(Lexer *lexer) {
    Parser parser = {0};
    parser._lexer = lexer;

    next_token_parser(&parser);
    next_token_parser(&parser);

    return parser;
}

void next_token_parser(Parser *parser) {
    parser->_current_token = parser->_peek_token;
    parser->_peek_token = next_token_lexer(&(*parser->_lexer));
}

Program *program_parser(Parser *parser) {
    return NULL;
}