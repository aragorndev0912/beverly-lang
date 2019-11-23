#include <stdio.h>
#include "./src/lexer.h"

int main(void) {

    Lexer lexer = new_lexer("let x = 5;");
    read_char_lexer(&lexer);

    return 0;
}