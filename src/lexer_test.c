#include "./lexer.h"
#include "./bool.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

void test_0() {
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

    printf("Lexer test has been complited.\n");
}

int main(void) {
    test_0();

    return 0;
}