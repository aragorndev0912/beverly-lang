#include "./lexer.h"
#include <string.h>
#include "./bool.h"

static const char *read_identifier(Lexer *lexer);
static bool is_letter(char ch);
static const char *get_slice_str(Lexer *lexer, int begin);
static void skip_space(Lexer *lexer);
static const *read_number(Lexer *lexer);
static bool is_digit(char ch);

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
            token = new_token(ASSIGN, lexer->_ch);
            break;

        case ';':
            token = new_token(SEMICOLON, lexer->_ch);
            break;

        case '(':
            token = new_token(LPAREN, lexer->_ch);
            break;

        case ')':
            token = new_token(RPAREN, lexer->_ch);
            break;
        
        case '+':
            token = new_token(PLUS, lexer->_ch);
            break;
        
        case '{':
            token = new_token(LBRACE, lexer->_ch);
            break;

        case '}':
            token = new_token(RBRACE, lexer->_ch);
            break;

        case 0:
            set_literal_token(&token, 0);
            set_type_token(&token, EOF);
        
        default:
            if (is_letter(lexer->_ch)) {
                set_literal_token(&token, read_identifier(lexer));
                set_type_token(&token, look_up_ident_token(&token));
                return token;
            }
            else if (is_digit(lexer->_ch)) {
                set_literal_token(&token, read_number(lexer));
                set_type_token(&token, INT);
                return token;
            }
            else
                token = new_token(ILLEGAL, lexer->_ch);
    }

    read_lexer(lexer);
    return token;
}

const char *read_identifier(Lexer *lexer) {
    int begin = lexer->_position;
    while (is_letter(lexer->_ch))
        read_char_lexer(lexer);

    return get_slice_str(lexer, begin);
}

static bool is_letter(char ch) {
    return ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch == '_'; 
}

static const char *get_slice_str(Lexer *lexer, int begin) {
    int end = lexer->_position;
    int diff = end - begin;
    char *str = (char *) malloc(sizeof(char) * diff+1);

    int index = 0;
    for (int k=begin; k < end; k++)
        str[0] = lexer->_input[k];

    return str;
}

static void skip_space(Lexer *lexer) {
    while (lexer->_ch == ' ' || lexer->_ch == '\t' || lexer->_ch == '\n' || lexer->_ch == '\r')
        read_char_lexer(lexer);
}

static const *read_number(Lexer *lexer) {
    int begin = lexer->_position;
    while (is_digit(lexer->_ch))
        read_char_lexer(lexer);

    return get_slice_str(lexer, begin);
}

static bool is_digit(char ch) {
    return ch >= '0' && ch <= '9';  
}