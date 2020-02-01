#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../ast/ast.h"
#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "../lib/bool.h"
#include "../object/object.h"
#include "../evaluator/evaluator.h"

//----------------------------------------------------------------------------------
// Firmas de funciones estaticas.
//----------------------------------------------------------------------------------
static void delete_data(Program *program, Lexer *lexer, Parser *parser);

static bool __test_evalIntegerExpression(void);

static Object __eval_frontEnd(const char *input);

static bool __test_evalIntegerObject(Object *obj, int expected);

static bool checkParserErrors(const Parser *const parser);

//----------------------------------------------------------------------------------
// Funcion principal.
//----------------------------------------------------------------------------------

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

static bool __test_evalIntegerExpression(void) {
    _TestInteger test[] = {
        {"5", 5},
        {"10", 10},
    }; 

    size_t size = sizeof(test) / sizeof(_TestInteger);
    for (size_t k=0; k < size; k++) {
        Object evaluate = __eval_frontEnd(test[k].input);
        if (evaluate._obj == NULL) {
            printf("Object value is NULL\n");
            return false;
        }
        if(!__test_evalIntegerObject(&evaluate, test[k].expected)) {
            free_object(&evaluate);
            return false;
        }

        free_object(&evaluate);
    }

    return true;
}

static Object __eval_frontEnd(const char *input) {
    Object result = (Object){._obj=NULL, ._type=OBJ_UNDEFINED};

    Lexer lexer = new_lexer(input);
    Parser parser = new_parser(&lexer);
    Program program = program_parser(&parser);
    if (checkParserErrors(&parser)) {
        delete_data(&program, &lexer, &parser);
        return result;
    }

    result = evaluation(&program);

    delete_data(&program, &lexer, &parser);
    return result;
}

bool __test_evalIntegerObject(Object *obj, int expected) {

    if (obj->_type != OBJ_INTEGER) {
        printf("Type object has been INTEGER type.\n");
        return false;
    }

    OInteger *integer = (OInteger *)obj->_obj; 
    if (integer->_value != expected) {
        printf("Value integer has been '%d'. got:'%d'\n", expected, integer->_value);
        return false;
    }

    return true;
}

static void delete_data(Program *program, Lexer *lexer, Parser *parser) {
    free_program(program);
    free_lexer(lexer);
    free_parser(parser);
}

static bool checkParserErrors(const Parser *const parser) {
    if (parser->error._errors == NULL) 
        return false;

    printf("parser has %ld errors.\n", parser->error._len);
    for (int k=0; k < parser->error._len; k++) 
        printf("%s", parser->error._errors[k]);

    return true;   
}
