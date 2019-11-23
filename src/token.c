#include "./token.h"
#include <string.h>
#include <stdlib.h>

Token new_token(TokenType type, const char *literal) {
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

void set_literal_token(Token *token, const char *literal) {
    token->_literal = literal;
}

TokenType look_up_ident_token(const Token *token) {
    if (strcmp(token->_literal, "fn") == 0) return BEV_FUNCTION;
    else if (strcmp(token->_literal, "let") == 0) return BEV_LET;
    else return BEV_IDENT;
}