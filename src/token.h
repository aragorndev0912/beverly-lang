#ifndef BEV_TOKEN_H
#define BEV_TOKEN_H

const char *BEV_ILLEGAL = "ILLEGAL";
const char *BEV_EOF = "EOF";

// Identificadores y  literales.
const char *BEV_IDENT = "IDENT"; // foo, x, y, a, ...
const char *BEV_INT = "INT"; // 1, 55, ...

// Operadores
const char *BEV_ASSIGN = "=";
const char *BEV_PLUS = "+";

// Delimitadores.
const char *BEV_COMMA = ",";
const char *BEV_SEMICOLON = ";";

const char *BEV_LPAREN = "(";
const char *BEV_RPAREN = ")";
const char *BEV_LBRACE = "{";
const char *BEV_RBRACE = "}";

// Palabras claves.
const char *BEV_FUNCTION = "FUNCTION";
const char *BEV_LET = "LET";

typedef const char *TokenType;

typedef struct Token {
    TokenType _type;
    const char *_literal;

} Token;

Token new_token(TokenType type, const char *literal);

void free_token(Token *token);

void set_type_token(Token *token, TokenType type);

void set_literal_token(Token *token, const char *literal);

TokenType look_up_ident_token(const Token *token);

#endif //BEV_TOKEN_H