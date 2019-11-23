#include "./lexer.h"
#include <string.h>

Lexer new_lexer(const char *input) {
    Lexer lexer = {0};
    lexer._input = input;
    read_lexer(&lexer);

    return lexer;
}

void read_lexer(Lexer *lexer) {
    if (lexer->_read_position >= strlen(lexer->_input)) 
        lexer->_ch = 0;
    else 
        lexer->_ch = lexer->_input[lexer->_read_position];

    lexer->_position = lexer->_read_position;
    lexer->_read_position++;
}

Token next_token_lexer(Lexer *lexer) {
    Token _token;

    

}