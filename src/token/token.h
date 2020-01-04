#ifndef _BEV_TOKEN_H
#define _BEV_TOKEN_H

extern char *BEV_UNDEFINED;
extern char *BEV_ILLEGAL;
extern char *BEV_EOF;

// Identificadores y  literales.
extern char *BEV_IDENT; // foo, x, y, a, ...
extern char *BEV_INT; // 1, 55, ...

// Operadores
extern char *BEV_ASSIGN;
extern char *BEV_PLUS;
extern char *BEV_MINUS;
extern char *BEV_NOT;
extern char *BEV_MULT;
extern char *BEV_DIV;
extern char *BEV_BANG;

extern char *BEV_LT;
extern char *BEV_GT;
extern char *BEV_EQUAL;
extern char *BEV_NOT_EQUAL;
extern char *BEV_DOUBLE_DOT;
extern char *BEV_ALLOCATE;

// Delimitadores.
extern char *BEV_COMMA;
extern char *BEV_SEMICOLON;

extern char *BEV_LPAREN;
extern char *BEV_RPAREN;
extern char *BEV_LBRACE;
extern char *BEV_RBRACE;

// Palabras claves.
extern char *BEV_FUNCTION;
extern char *BEV_LET;
extern char *BEV_TRUE;
extern char *BEV_FALSE;
extern char *BEV_IF;
extern char *BEV_ELSE;
extern char *BEV_RETURN;

typedef char *TokenType;

typedef struct Token {
    TokenType _type;
    char *_literal;

} Token;

Token new_token(TokenType type, char *literal);

void free_token(Token *token);

void set_type_token(Token *token, TokenType type);

void set_literal_token(Token *token, char *literal);

TokenType look_up_ident_token(const Token *token);

#endif //_BEV_TOKEN_H
