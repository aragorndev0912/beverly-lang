#include "./lexer.h"
#include "./bool.h"
#include <string.h>
#include <stdlib.h>

static char *read_identifier(Lexer *lexer);
static bool is_letter(char ch);
static char *get_slice_str(Lexer *lexer, int begin);
static void skip_space(Lexer *lexer);
static char *read_number(Lexer *lexer);
static bool is_digit(char ch);
static char *to_str(const Lexer *lexer, size_t n);

Lexer new_lexer(const char *input) {
    Lexer lexer = {0};
    lexer._input = input;
    read_char_lexer(&lexer);

    return lexer;
}

void free_lexer(Lexer *lexer) {
    // Falta implementacion.
}

void read_char_lexer(Lexer *lexer) {
    if (lexer->_read_position >= strlen(lexer->_input)) 
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
            token = new_token(BEV_ASSIGN, to_str(lexer, 2));
            break;

        case ';':
            token = new_token(BEV_SEMICOLON, to_str(lexer, 2));
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
        
        case '{':
            token = new_token(BEV_LBRACE, to_str(lexer, 2));
            break;

        case '}':
            token = new_token(BEV_RBRACE, to_str(lexer, 2));
            break;

        case 0:
            set_literal_token(&token, 0);
            set_type_token(&token, BEV_EOF);
        
        default:
            if (is_letter(lexer->_ch)) {
                set_literal_token(&token, read_identifier(lexer));
                set_type_token(&token, look_up_ident_token(&token));
                return token;
            }
            else if (is_digit(lexer->_ch)) {
                set_literal_token(&token, read_number(lexer));
                set_type_token(&token, BEV_INT);
                return token;
            }
            else
                token = new_token(BEV_ILLEGAL, to_str(lexer, 2));
    }

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

    int index = 0;
    for (int k=begin; k < end; k++)
        str[index++] = lexer->_input[k];

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
    str[0] = lexer->_ch;
    str[1] = '\0';

    return str;
}