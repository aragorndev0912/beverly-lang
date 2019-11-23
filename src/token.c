#include "./token.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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


typedef struct __ArrayLiteral {
    char *_literal;
    struct __ArrayLiteral *next;

} __ArrayLiteral;

static void add_arrayliteral(__ArrayLiteral **array, char * literal) {
    printf("add: %s\n", literal);
    if (array == NULL) {
        (*array) = (__ArrayLiteral *) malloc(sizeof(__ArrayLiteral));
        (*array)->_literal = literal;
        (*array)->next = NULL;
    }
    else {
        __ArrayLiteral *aux = (*array);
        while (aux->next != NULL)
            aux->next = aux->next;

        aux->next = (__ArrayLiteral *) malloc(sizeof(__ArrayLiteral));
        aux->next->_literal = literal;
        aux->next->next = NULL;
    }
}

static void del_arrayliteral(__ArrayLiteral **array) {
    __ArrayLiteral *aux = NULL;
    while ((*array) != NULL) {
        aux = (*array); 
        (*array) = (*array)->next;
        printf("DEL literal: %s\n", aux->_literal);
        free(aux->_literal);
        aux->_literal = NULL;
        free(aux);
        aux = NULL;
    }
}

static __ArrayLiteral *array_literal = NULL;

Token new_token(TokenType type, char *literal) {
    Token token = {0};
    token._type = type;
    token._literal = literal;
    printf("literal: %s\n", literal);
    // Agregar a un array para liberar la memoria reservada.
    add_arrayliteral(&array_literal, token._literal);

    return token;
}

void free_token(Token *token) {
    // del_arrayliteral(&array_literal);
    printf("DEL: %s\n", token->_literal);
    if (token->_literal != NULL) {
        printf("DEL: %s\n", token->_literal);
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