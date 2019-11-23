#include "./token.h"
#include <string.h>
#include <stdlib.h>

Token new_token(TokenType type, char literal) {
    Token token = {0};
    token._type = type;
    token._literal = literal;

    return token;
}

void free_token(Token *token) {
    // Falta implementacion.
}

void set_type_token(Token *token, TokenType type) {
    token->_type = type;
}

void set_literal_token(Token *token, char *literal) {
    token->_literal = literal;
}

TokenType look_up_ident_token(const Token *token) {
    if (token->_literal == "fn") return FUNCTION;
    else if (token->_literal == "let") return LET;
    else return IDENT;
}