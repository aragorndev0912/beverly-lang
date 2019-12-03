#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "ast.h"
#include "lexer.h"
#include "parser.h"

static void __test_letStatement(void) {
    const char *input = 
    "let x = 5;\n\
    let y = 10;\n\
    let foobar = 838383;";

    Lexer lexer = new_lexer(input);
    Parser parser = new_parser(&lexer);

    Program *program = program_parser(&parser); 
    if (program == NULL) {
        free_lexer(&lexer);
        printf("Error, program is NULL");
        return;
    }
}

int main(void) {
    __test_letStatement();

    return 0;
}