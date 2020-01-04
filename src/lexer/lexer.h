#ifndef _BEV_LEXER_H
#define _BEV_LEXER_H

#include "../token/token.h"
#include <stddef.h>

typedef struct Lexer {
    const char *_input;
    size_t _len_input;
    int _position;
    int _read_position;
    char _ch;

} Lexer;

Lexer new_lexer(const char *input);

void free_lexer(Lexer *lexer);

void read_char_lexer(Lexer *lexer);

Token next_token_lexer(Lexer *lexer);


#endif //_BEV_LEXER_H
