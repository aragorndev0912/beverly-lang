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

static bool __test_booleanExpression(void);

static bool __test_evalBooleanObject(Object *obj, bool expected);

static bool __test_bangPrefixObject(void);

static bool __test_ifElseObject(void);

//----------------------------------------------------------------------------------
// Funcion principal.
//----------------------------------------------------------------------------------

int main(void) {
    assert(__test_evalIntegerExpression());
    printf("__test_evalIntegerExpression (COMPLETED).\n");

    assert(__test_booleanExpression());
    printf("__test_booleanExpression (COMPLETED).\n");

    assert(__test_bangPrefixObject());
    printf("__test_bangPrefixObject (COMPLETED).\n");

    assert(__test_ifElseObject());
    printf("__test_ifElseObject (COMPLETED).\n");

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
    _TestInteger tests[] = {
        (_TestInteger){.input="10", .expected=10},
        (_TestInteger){.input="25", .expected=25},
        (_TestInteger){.input="-10", .expected=-10},
        (_TestInteger){.input="-25", .expected=-25},
        (_TestInteger){.input="5 + 5 + 5 + 5 + 10", .expected=30},
        (_TestInteger){.input="2 * 2 * 2 * 2 * 2", .expected=32},
        (_TestInteger){.input="-50 + 100 + -50", .expected=0},
        (_TestInteger){.input="5 * 2 + 10", .expected=20},
        (_TestInteger){.input="5 + 2 * 10", .expected=25},
        (_TestInteger){.input="20 + 2 * -10", .expected=0},
        (_TestInteger){.input="50 / 2 * 2 + 10", .expected=60},
        (_TestInteger){.input="2 * (5 + 10)", .expected=30},
        (_TestInteger){.input="3 * 3 * 3 + 10", .expected=37},
        (_TestInteger){.input="3 * (3 * 3) + 10", .expected=37},
        (_TestInteger){.input="(5 + 10 * 2 + 15 / 3) * 2 + -10", .expected=50},
    }; 

    size_t size = sizeof(tests) / sizeof(_TestInteger);
    for (size_t k=0; k < size; k++) {
        Object evaluate = __eval_frontEnd(tests[k].input);
        if (evaluate._obj == NULL) {
            printf("Object value is NULL\n");
            return false;
        }
        if(!__test_evalIntegerObject(&evaluate, tests[k].expected)) {
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
        printf("Type object has been not INTEGER type.\n");
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

typedef struct _TestBoolean {
    const char *input;
    bool expected;

} _TestBoolean;

static bool __test_booleanExpression(void) {
    _TestBoolean tests[] = {
        (_TestBoolean){.input="true", .expected=true},
        (_TestBoolean){.input="false", .expected=false},
        (_TestBoolean){.input="1 < 2", .expected=true},
        (_TestBoolean){.input="1 > 2", .expected=false},
        (_TestBoolean){.input="1 < 1", .expected=false},
        (_TestBoolean){.input="1 > 1", .expected=false},
        (_TestBoolean){.input="1 == 1",.expected=true},
        (_TestBoolean){.input="1 != 1",.expected=false},
        (_TestBoolean){.input="1 == 2",.expected=false},
        (_TestBoolean){.input="1 != 2",.expected=true},
        (_TestBoolean){.input="true == true", .expected=true},
        (_TestBoolean){.input="false == false", .expected=true},
        (_TestBoolean){.input="true == false", .expected=false},
        (_TestBoolean){.input="true != false", .expected=true},
        (_TestBoolean){.input="false != true", .expected=true},
        (_TestBoolean){.input="(1 < 2) == true", .expected=true},
        (_TestBoolean){.input="(1 < 2) == false", .expected=false},
        (_TestBoolean){.input="(1 > 2) == true", .expected=false},
        (_TestBoolean){.input="(1 > 2) == false", .expected=true},
    };

    size_t size = sizeof(tests) / sizeof(_TestBoolean);
    for (size_t k=0; k < size; k++) {
        Object evaluate = __eval_frontEnd(tests[k].input);
        if (evaluate._obj == NULL) {
            printf("Object value is NULL\n");
            return false;
        }
        if(!__test_evalBooleanObject(&evaluate, tests[k].expected)) {
            free_object(&evaluate);
            return false;
        }

        free_object(&evaluate);
    }

    return true;
}

static bool __test_evalBooleanObject(Object *obj, bool expected) {
   if (obj->_type != OBJ_BOOLEAN) {
        printf("Type object has been not BOOLEAN type.\n");
        return false;
    }

    OBoolean *boolean = (OBoolean *)obj->_obj; 
    if (boolean->_value != expected) {
        printf("Value boolean has been '%d'. got:'%d'\n", expected, boolean->_value);
        return false;
    }

    return true; 
}

static bool __test_bangPrefixObject(void) {
    _TestBoolean tests[] = {
        (_TestBoolean){.input="not true", .expected=false},
        (_TestBoolean){.input="not false", .expected=true},
        (_TestBoolean){.input="not 6", .expected=false},
        (_TestBoolean){.input="not not true", .expected=true},
        (_TestBoolean){.input="not not false", .expected=false},
        (_TestBoolean){.input="not not 6", .expected=true},
    };

    size_t size = sizeof(tests) / sizeof(_TestBoolean);
    for (size_t k=0; k < size; k++) {
        Object evaluate = __eval_frontEnd(tests[k].input);
        if (evaluate._obj == NULL) {
            printf("Object value is NULL\n");
            return false;
        }
        if(!__test_evalBooleanObject(&evaluate, tests[k].expected)) {
            free_object(&evaluate);
            return false;
        }

        free_object(&evaluate);
    }

    return true;
}

typedef struct _IfTest {
    const char *input;
    TypeObject type;
    int value;

} _IfTest;

static bool __test_ifElseObject(void) {
    _IfTest tests[] = {
        (_IfTest){.input="if (true) { 10 }", .type=OBJ_INTEGER, .value=10},
        (_IfTest){.input="if (false) { 10 }", .type=OBJ_NULL, .value=0},
        (_IfTest){.input="if (true) { 10 }", .type=OBJ_INTEGER, .value=10},
        (_IfTest){.input="if (1 < 2) { 10 }", .type=OBJ_INTEGER, .value=10},
        (_IfTest){.input="if (1 > 2) { 10 }", .type=OBJ_NULL, .value=0},
        (_IfTest){.input="if (1 > 2) { 10 } else { 20 }", .type=OBJ_INTEGER, .value=20},
        (_IfTest){.input="if (1 < 2) { 10 } else { 20 }", .type=OBJ_INTEGER, .value=10},
    };

    size_t size = sizeof(tests) / sizeof(_IfTest);
    for (size_t k=0; k < size; k++) {
        Object evaluate = __eval_frontEnd(tests[k].input);
        if (evaluate._obj == NULL) {
            printf("Object value is NULL\n");
            return false;
        }
        
        if (evaluate._type != tests[k].type) {
            printf("Data type is not equal.\n");
            free_object(&evaluate);
            return false;
        } 

        if (evaluate._type == OBJ_INTEGER && ((OInteger *)evaluate._obj)->_value != tests[k].value) {
            printf("Value not do match '%d'. got='%d'.\n", tests[k].value, ((OInteger *)evaluate._obj)->_value);
            free_object(&evaluate);
            return false;
        }

        free_object(&evaluate);
    }
    return true;
}
