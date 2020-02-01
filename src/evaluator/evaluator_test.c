#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../ast/ast.h"
#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "../lib/bool.h"

//----------------------------------------------------------------------------------
// Firmas de funciones estaticas.
//----------------------------------------------------------------------------------

bool __test_evalIntegerExpression(void);



int main(void) {
    assert(__test_evalIntegerExpression());
    printf("__test_evalIntegerExpression (COMPLETED).\n");

    return 0;
}

//----------------------------------------------------------------------------------
// Implementacion de funciones estaticas.
//----------------------------------------------------------------------------------
typedef struct _TestInteger {
    const char *input;
    int expected;

} _TestInteger; 

bool __test_evalIntegerExpression(void) {
    _TestInteger test[] = {
        {"5", 5},
        {"10", 10},
    }; 

    size_t size = sizeog(test) / sizeof(_TestInteger);
    for (size_t k=0; k < size; k++) {

    }

    return true;
}
