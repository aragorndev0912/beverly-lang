#include "./lexer.h"
#include "./bool.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

void test_0(void) {
    const char *input ="=+(){},;";

    const char *tests[9][2] = {
        {BEV_ASSIGN, "="},
        {BEV_PLUS, "+"},
        {BEV_LPAREN, "("},
        {BEV_RPAREN, ")"},
        {BEV_LBRACE, "{"},
        {BEV_RBRACE, "}"},
        {BEV_COMMA, ","},
        {BEV_SEMICOLON, ";"},
        {BEV_EOF, ""}
    };

    Lexer lexer = new_lexer(input);

    Token token = next_token_lexer(&lexer);
    int index = 0;
    while (token._type != NULL && token._type != BEV_EOF) {
        assert(strcmp(tests[index][0], token._type) == 0);
        assert(strcmp(tests[index][1], token._literal) == 0);
        index++;

        free_token(&token);
        token = next_token_lexer(&lexer);
    }

    if (token._type != NULL)
        free_token(&token);

    printf("Lexer test(0) has been complited.\n");
}

void test_1(void) {
    const char *input = 
    "let five = 5; \n\
    let ten = 10;\n\
    \n\
    let add = fn(x, y) {\n\
        x + y;\n\
    };\n\
    \n\
    let result = add(five, ten);\n\
    ";
    // printf("%s\n", input);

    const char *tests[37][2] = {
        {BEV_LET, "let"}, 
        {BEV_IDENT, "five"}, 
        {BEV_ASSIGN, "="}, 
        {BEV_INT, "5"}, 
        {BEV_SEMICOLON, ";"}, 
        {BEV_LET, "let"}, 
        {BEV_IDENT, "ten"}, 
        {BEV_ASSIGN, "="}, 
        {BEV_INT, "10"}, 
        {BEV_SEMICOLON, ";"}, 
        {BEV_LET, "let"}, 
        {BEV_IDENT, "add"}, 
        {BEV_ASSIGN, "="}, 
        {BEV_FUNCTION, "fn"}, 
        {BEV_LPAREN, "("}, 
        {BEV_IDENT, "x"}, 
        {BEV_COMMA, ","}, 
        {BEV_IDENT, "y"}, 
        {BEV_RPAREN, ")"}, 
        {BEV_LBRACE, "{"}, 
        {BEV_IDENT, "x"}, 
        {BEV_PLUS, "+"}, 
        {BEV_IDENT, "y"}, 
        {BEV_SEMICOLON, ";"}, 
        {BEV_RBRACE, "}"}, 
        {BEV_SEMICOLON, ";"}, 
        {BEV_LET, "let"},
        {BEV_IDENT, "result"}, 
        {BEV_ASSIGN, "="}, 
        {BEV_IDENT, "add"}, 
        {BEV_LPAREN, "("}, 
        {BEV_IDENT, "five"}, 
        {BEV_COMMA, ","}, 
        {BEV_IDENT, "ten"}, 
        {BEV_RPAREN, ")"}, 
        {BEV_SEMICOLON, ";"}, 
        {BEV_EOF, ""} 
    };

    Lexer lexer = new_lexer(input);

    Token token = next_token_lexer(&lexer);
    int index = 0;
    while (token._type != NULL && token._type != BEV_EOF) {
        assert(strcmp(tests[index][0], token._type) == 0);
        assert(strcmp(tests[index][1], token._literal) == 0);
        index++;

        free_token(&token);
        token = next_token_lexer(&lexer);
    }

    if (token._type != NULL)
        free_token(&token);

    printf("Lexer test(1) has been complited.\n");

}

int main(void) {
    test_0();
    test_1();

    return 0;
}