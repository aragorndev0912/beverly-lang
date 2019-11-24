#include "./token.h"
#include <string.h>
#include <stdlib.h>

char *BEV_ILLEGAL = "ILLEGAL";
char *BEV_EOF = "EOF";

// Identificadores y  literales.
char *BEV_IDENT = "IDENT"; // foo, x, y, a, ...
char *BEV_INT = "INT"; // 1, 55, ...

// Operadores
char *BEV_ASSIGN = "=";
char *BEV_PLUS = "+";

// Delimitadores.
char *BEV_COMMA = ",";
char *BEV_SEMICOLON = ";";

char *BEV_LPAREN = "(";
char *BEV_RPAREN = ")";
char *BEV_LBRACE = "{";
char *BEV_RBRACE = "}";

// Palabras claves.
char *BEV_FUNCTION = "FUNCTION";
char *BEV_LET = "LET";

Token new_token(TokenType type, char *literal) {
    Token token = {0};
    token._type = type;
    token._literal = literal;

    return token;
}

void free_token(Token *token) {
    if (token->_literal != NULL && token->_type != BEV_EOF) {
        free(token->_literal);
        token->_literal = NULL;
    }
}

void set_type_token(Token *token, TokenType type) {
    token->_type = type;
}

void set_literal_token(Token *token, char *literal) {
    token->_literal = literal;
}

TokenType look_up_ident_token(const Token *token) {
    if (strcmp(token->_literal, "fn") == 0) return BEV_FUNCTION;
    else if (strcmp(token->_literal, "let") == 0) return BEV_LET;
    else return BEV_IDENT;
}