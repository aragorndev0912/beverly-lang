#include "./token.h"

Token new_token(TokenType type, const char *literal) {
    Token token = {0};
    token._type = type;
    token._literal = literal;

    return token;
}