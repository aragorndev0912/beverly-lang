#include <stdio.h>
#include "./src/lexer.h"
#include <stdlib.h>
#include <errno.h>

int main(void) {

    Lexer lexer = new_lexer("let x = 5;");
    
    Token token = next_token_lexer(&lexer);

    free_token(&token);
    free_lexer(&lexer); // libera la memoria.

    return 0;
}