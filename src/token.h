#ifndef TOKEN_H
#define TOKEN_H

const char *ILLEGAL = "ILLEGAL";
const char *EOF = "EOF";

// Identificadores y  literales.
const char *IDENT = "IDENT"; // foo, x, y, a, ...
const char *INT = "INT"; // 1, 55, ...

// Operadores
const char *ASSIGN = "=";
const char *PLUS = "+";

// Delimitadores.
const char *COMMA = ",";
const char *SEMICOLON = ";";

const char *LPAREN = "(";
const char *RPAREN = ")";
const char *LBRACE = "{";
const char *RBRACE = "}";

// Palabras claves.
const char *FUNCTION = "FUNCTION";
const char *LET = "LET";

typedef char TokenType;

typedef struct Token {
    TokenType _type;
    const char *_literal;

} Token;

Token new_token(TokenType type, char literal);

#endif //TOKEN_H