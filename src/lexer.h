#ifndef LEXER_H
#define LEXER_H

#include "./token.h"

typedef struct Lexer {
    const char *_input;
    int _position;
    int _read_position;
    char _ch;

} Lexer;

Lexer new_lexer(const char *input);

void read_lexer(Lexer *lexer);

Token next_token_lexer(Lexer *lexer);

#endif //LEXER_H