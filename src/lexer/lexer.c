#include "../lexer/lexer.h"
#include "../lib/bool.h"
#include <string.h>
#include <stdlib.h>

#include <stdio.h>

static char *read_identifier(Lexer *lexer);
static bool is_letter(char ch);
static char *get_slice_str(Lexer *lexer, int begin);
static void skip_space(Lexer *lexer);
static char *read_number(Lexer *lexer);
static bool is_digit(char ch);
static char *to_str(const Lexer *lexer, size_t n);
static char peek_char(const Lexer *lexer);

Lexer new_lexer(const char *input) {
    Lexer lexer = (Lexer){0};
    lexer._input = input;
    lexer._len_input = strlen(lexer._input);
    read_char_lexer(&lexer);

    return lexer;
}

void free_lexer(Lexer *lexer) {
    // Falta implementacion.
}

void read_char_lexer(Lexer *lexer) {
    if (lexer->_read_position >= lexer->_len_input) 
        lexer->_ch = 0;
    else 
        lexer->_ch = lexer->_input[lexer->_read_position];

    lexer->_position = lexer->_read_position;
    lexer->_read_position++;
}

Token next_token_lexer(Lexer *lexer) {
    Token token = {0};
    skip_space(lexer);

    switch (lexer->_ch) {
        case '=':
            if (peek_char(lexer) == '=') {
                char *ch = (char *) malloc(sizeof(char) * 3);
                if (ch == NULL) {
                    // Falta implementar limpieza.
                }
                ch[0] = lexer->_ch;
                read_char_lexer(lexer);
                ch[1] = lexer->_ch;
                ch[2] = '\0';
                token = new_token(BEV_EQUAL, ch);
            }
            else 
                token = new_token(BEV_ASSIGN, to_str(lexer, 2));
            break;
        
        case '!':
            if (peek_char(lexer) == '=') {
                char *ch = (char *) malloc(sizeof(char) * 3);
                if (ch == NULL) {
                    // Falta implementar limpieza.
                }
                ch[0] = lexer->_ch;
                read_char_lexer(lexer);
                ch[1] = lexer->_ch;
                ch[2] = '\0';
                token = new_token(BEV_NOT_EQUAL, ch);
            }
            else
                token = new_token(BEV_BANG, to_str(lexer, 2));
            break;
        
        case ':':
            if (peek_char(lexer) == '=') {
                // allocate.
                char *ch = (char *) malloc(sizeof(char) * 3);
                if (ch == NULL) {
                    // Falta implementar limpieza.
                }
                ch[0] = lexer->_ch;
                read_char_lexer(lexer);
                ch[1] = lexer->_ch;
                ch[2] = '\0';
                token = new_token(BEV_ALLOCATE, ch);
            }
            else {
                token = new_token(BEV_DOUBLE_DOT, to_str(lexer, 2));
            }
            break;

        case ';':
            token = new_token(BEV_SEMICOLON, to_str(lexer, 2));
            break;
        
        case ',':
            token = new_token(BEV_COMMA, to_str(lexer, 2));
            break;

        case '(':
            token = new_token(BEV_LPAREN, to_str(lexer, 2));
            break;

        case ')':
            token = new_token(BEV_RPAREN, to_str(lexer, 2));
            break;
        
        case '+':
            token = new_token(BEV_PLUS, to_str(lexer, 2));
            break;
        
        case '-':
            token = new_token(BEV_MINUS, to_str(lexer, 2));
            break;
        
        case '*':
            token = new_token(BEV_MULT, to_str(lexer, 2));
            break;
        
        case '<':
            token = new_token(BEV_LT, to_str(lexer, 2));
            break;
        
        case '>':
            token = new_token(BEV_GT, to_str(lexer, 2));
            break;
        
        case '/':
            token = new_token(BEV_DIV, to_str(lexer, 2));
            break;
        
        case '{':
            token = new_token(BEV_LBRACE, to_str(lexer, 2));
            break;

        case '}':
            token = new_token(BEV_RBRACE, to_str(lexer, 2));
            break;

        case 0:
            token = new_token(BEV_EOF, 0);
            break;
        
        default:
            if (is_letter(lexer->_ch)) {
                set_literal_token(&token, read_identifier(lexer));
                set_type_token(&token, look_up_ident_token(&token));
                return token;
            }
            else if (is_digit(lexer->_ch)) {
                token = new_token(BEV_INT, read_number(lexer));
                return token;
            }
            else
                token = new_token(BEV_ILLEGAL, to_str(lexer, 2));
    }

    if (token._type != BEV_EOF)
        read_char_lexer(lexer);
    
    return token;
}

static char *read_identifier(Lexer *lexer) {
    int begin = lexer->_position;
    while (is_letter(lexer->_ch))
        read_char_lexer(lexer);

    return get_slice_str(lexer, begin);
}

static bool is_letter(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch == '_'); 
}

static char *get_slice_str(Lexer *lexer, int begin) {
    int end = lexer->_position;
    int diff = end - begin;
    char *str = (char *) malloc(sizeof(char) * diff+1);
    if (str == NULL) {
        // Falta implementar limpieza.
    }
    int index = 0;
    for (int k=begin; k < end; k++)
        str[index++] = lexer->_input[k];

    str[diff] = '\0';

    return str;
}

static void skip_space(Lexer *lexer) {
    while (lexer->_ch == ' ' || lexer->_ch == '\t' || lexer->_ch == '\n' || lexer->_ch == '\r')
        read_char_lexer(lexer);
}

static char *read_number(Lexer *lexer) {
    int begin = lexer->_position;
    while (is_digit(lexer->_ch))
        read_char_lexer(lexer);

    return get_slice_str(lexer, begin);
}

static bool is_digit(char ch) {
    return ch >= '0' && ch <= '9';  
}

static char *to_str(const Lexer *lexer, size_t n) {
    char *str = (char *) malloc(sizeof(char) * n);
    if (str == NULL) {
        // Falta implementar limpieza.
    }
    str[0] = lexer->_ch;
    str[1] = '\0';

    return str;
}

static char peek_char(const Lexer *lexer) {
    if (lexer->_read_position >= lexer->_len_input)
        return 0;
    else
        return lexer->_input[lexer->_read_position];
}
