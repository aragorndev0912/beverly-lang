#ifndef BEV_LEXER_H
#define BEV_LEXER_H

#include "./token.h"

typedef struct Lexer {
    const char *_input;
    int _position;
    int _read_position;
    char _ch;

} Lexer;

Lexer new_lexer(const char *input);

void free_lexer(Lexer *lexer);

void read_char_lexer(Lexer *lexer);

Token next_token_lexer(Lexer *lexer);


#endif //BEV_LEXER_H