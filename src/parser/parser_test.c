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
static void delete_data(Program *program, Lexer *lexer, Parser *parser);

static bool let_statement(const LetStatement *const let_stmt, const char *name);

static bool checkParserErrors(const Parser *const parser);

static bool __test_letStatement(void);

static bool __test_returnStatement(void);

static bool __test_identifier(void);

static bool __test_integerLiteral(const Expression *expression, int value);

static bool __test_prefixExpression(void);

static bool __test_infixExpression(void);

static bool __test_OperatorPrecedence(void);

static bool __test_BooleanLiteral(const Expression *expression, bool value);

//----------------------------------------------------------------------------------
// Funcion main.
//----------------------------------------------------------------------------------
int main(void) {
    assert(__test_letStatement());
    printf("__test_letStatement (COMPLETED).\n");

    assert(__test_returnStatement());
    printf("__test_returnStatement (COMPLETED).\n");

    assert(__test_identifier());
    printf("__test_identifier (COMPLETED).\n");

    assert(__test_prefixExpression());
    printf("__test_prefixExpression (COMPLETED).\n");

    assert(__test_infixExpression());
    printf("__test_infixExpression (COMPLETED).\n");

    assert(__test_OperatorPrecedence());
    printf("__test_OperatorPrecedence (COMPLETED).\n");

    return 0;
}

//----------------------------------------------------------------------------------
// Implementacion de funciones estaticas.
//----------------------------------------------------------------------------------
typedef struct OperatorTest {
    const char *input;
    const char *expected;

} OperatorTest;

static bool __test_OperatorPrecedence(void) {
    OperatorTest tests[] = {
        (OperatorTest) {.input="-a * b", .expected="((-a) * b)"},
        (OperatorTest) {.input="not-a", .expected="(not(-a))"},
        (OperatorTest) {.input="a + b + c", .expected="((a + b) + c)"},
        (OperatorTest) {.input="a + b - c", .expected="((a + b) - c)"},
        (OperatorTest) {.input="a * b * c", .expected="((a * b) * c)"},
        (OperatorTest) {.input="a * b / c", .expected="((a * b) / c)"},
        (OperatorTest) {.input="a + b / c", .expected="(a + (b / c))"},
        (OperatorTest) {.input="a + b * c + d / e - f", .expected="(((a + (b * c)) + (d / e)) - f)"},
        (OperatorTest) {.input="3 + 4; -5 * 5", .expected="(3 + 4)((-5) * 5)"},
        (OperatorTest) {.input="5 > 4 == 3 < 4", .expected="((5 > 4) == (3 < 4))"},
        (OperatorTest) {.input="5 < 4 != 3 > 4", .expected="((5 < 4) != (3 > 4))"},
        (OperatorTest) {.input="3 + 4 * 5 == 3 * 1 + 4 * 5", .expected="((3 + (4 * 5)) == ((3 * 1) + (4 * 5)))"},
        (OperatorTest) {.input="3 + 4 * 5 == 3 * 1 + 4 * 5", .expected="((3 + (4 * 5)) == ((3 * 1) + (4 * 5)))"},
        (OperatorTest) {.input="true", .expected="true"},
        (OperatorTest) {.input="false", .expected="false"},
        (OperatorTest) {.input="3 > 5 == false", .expected="((3 > 5) == false)"},
        (OperatorTest) {.input="3 < 5 == true", .expected="((3 < 5) == true)"},
        (OperatorTest) {.input="1 + (2 + 3) + 4", .expected="((1 + (2 + 3)) + 4)"},
        (OperatorTest) {.input="(5 + 5) * 2", .expected="((5 + 5) * 2)"},
        (OperatorTest) {.input="2 / (5 + 5)", .expected="(2 / (5 + 5))"},
        (OperatorTest) {.input="-(5 + 5)", .expected="(-(5 + 5))"},
        (OperatorTest) {.input="not(true == true)", .expected="(not(true == true))"},
    };

    size_t len = sizeof(tests) / sizeof(OperatorTest);
    for (size_t k=0; k < len; k++) {
        Lexer lexer = new_lexer(tests[k].input);
        Parser parser = new_parser(&lexer);
        Program program = program_parser(&parser);
        if (checkParserErrors(&parser)) {
            delete_data(&program, &lexer, &parser);
            return false;
        }

        const char *__string = string_program(&program);
        if (__string == NULL) {
            printf("__string is equal to NULL.\n");
            delete_data(&program, &lexer, &parser);
            return false;
        }

        if (strcmp(__string, tests[k].expected) != 0) {
            printf("expected='%s', got='%s'.\n", tests[k].expected, __string);
            delete_data(&program, &lexer, &parser);
            return false;
        }

        delete_data(&program, &lexer, &parser);
    }


    return true;
}


typedef struct InfixTest {
    const char *input;
    int left;
    const char *operator;
    int right;

} InfixTest;

static bool __test_infixExpression(void) {
    InfixTest infix_test[] = {
        (InfixTest) {.input="10 + 8", .left=10, .operator="+", .right=8},
        (InfixTest) {.input="23 - 5", .left=23, .operator="-", .right=5},
        (InfixTest) {.input="123 * 46", .left=123, .operator="*", .right=46},
        (InfixTest) {.input="88 / 99", .left=88, .operator="/", .right=99},
        (InfixTest) {.input="54 > 5", .left=54, .operator=">", .right=5},
        (InfixTest) {.input="5 < 54", .left=5, .operator="<", .right=54},
        (InfixTest) {.input="40 == 12", .left=40, .operator="==", .right=12},
        (InfixTest) {.input="13 != 13", .left=13, .operator="!=", .right=13},
        (InfixTest) {.input="true == true", .left=true, .operator="==", .right=true},
        (InfixTest) {.input="true != false", .left=true, .operator="!=", .right=false},
        (InfixTest) {.input="false != false", .left=false, .operator="!=", .right=false},
    };

    size_t len = sizeof(infix_test) / sizeof(InfixTest);
    for (size_t k=0; k < len; k++) {
        Lexer lexer = new_lexer(infix_test[k].input);
        Parser parser = new_parser(&lexer);
        Program program = program_parser(&parser);
        if (checkParserErrors(&parser)) {
            delete_data(&program, &lexer, &parser);
            return false;
        }

        if (program._statements == NULL) {
            printf("Error, program is NULL\n");
            delete_data(&program, &lexer, &parser);
            return false;
        }

        if (program._len != 1) {
            printf("Error, program._len has been 1 statements. got=%d.\n", program._len);
            delete_data(&program, &lexer, &parser);
            return false;
        }

        ExpressionStatement *aux_exprStmt = ((ExpressionStatement *)program._statements[0]._ptr);
        InfixExpression *aux_infixExpression = ((InfixExpression *)aux_exprStmt->_expression._ptr);

        switch (aux_infixExpression->_left._type) {
            case EXPR_INTEGER:
                if (!__test_integerLiteral(&aux_infixExpression->_left, infix_test[k].left)) {
                    delete_data(&program, &lexer, &parser);
                    return false;
                }
                break;
            
            case EXPR_BOOLEAN:
                if (!__test_BooleanLiteral(&aux_infixExpression->_left, (bool) infix_test[k].left)) {
                    delete_data(&program, &lexer, &parser);
                    return false;
                }
                break;
            
            default:
                printf("Error ExpressionType: %d\n", aux_infixExpression->_left._type);
                return false;
        }

        if (strcmp(aux_infixExpression->_operator, infix_test[k].operator) != 0) {
            printf("expr.operator is not '%s'. got=%s\n", infix_test[k].operator, aux_infixExpression->_operator);
            delete_data(&program, &lexer, &parser);
            return false;
        }

        switch(aux_infixExpression->_right._type) {
            case EXPR_INTEGER:
                if (!__test_integerLiteral(&aux_infixExpression->_right, infix_test[k].right)) {
                    delete_data(&program, &lexer, &parser);
                    return false;
                }
                break;
            
            case EXPR_BOOLEAN:
                if (!__test_BooleanLiteral(&aux_infixExpression->_right, (bool) infix_test[k].right)) {
                    delete_data(&program, &lexer, &parser);
                    return false;
                }
                break;
            
            default:
                printf("Error ExpressionType: %d\n", aux_infixExpression->_right._type);
                return false;
        }

        delete_data(&program, &lexer, &parser);
    }

    
    return true;
}

typedef struct PrefixTest {
    const char *input;
    const char *operator;
    int   value;

} PrefixTest;

static bool __test_prefixExpression(void) {
    PrefixTest prefixTest[2] = {
        (PrefixTest) { .input="not 9", .operator="not", 9 },
        (PrefixTest) { .input="-45", .operator="-", 45 },
    };

    for (int k=0; k < 2; k++) {
        Lexer lexer = new_lexer(prefixTest[k].input);
        Parser parser = new_parser(&lexer);
        Program program = program_parser(&parser);
        if (checkParserErrors(&parser)) {
            delete_data(&program, &lexer, &parser);
            return false;
        }

        if (program._statements == NULL) {
            printf("Error, program is NULL\n");
            delete_data(&program, &lexer, &parser);
            return false;
        }

        if (program._len != 1) {
            printf("Error, program._len has been 1 statements. got=%d.\n", program._len);
            delete_data(&program, &lexer, &parser);
            return false;
        }

        ExpressionStatement *aux_exprStmt = ((ExpressionStatement *)program._statements[0]._ptr);
        PrefixExpression *aux_prefixExpression = ((PrefixExpression *)aux_exprStmt->_expression._ptr);

        if (strcmp(aux_prefixExpression->_operator, prefixTest[k].operator) != 0) {
            printf("Error, operator not %s. got=%s\n", prefixTest[k].operator, aux_prefixExpression->_operator);
            delete_data(&program, &lexer, &parser);
            return false;
        } 

        if (!__test_integerLiteral(&aux_prefixExpression->_right, prefixTest[k].value)) {
            delete_data(&program, &lexer, &parser);
            return false;
        }

        delete_data(&program, &lexer, &parser);
    }

    
    return true;
}

static bool __test_BooleanLiteral(const Expression *expression, bool value) {
    Boolean *boolean = (Boolean *)expression->_ptr;

    if (boolean->_value != value) {
        printf("boolean->_value not %d, got='%d'.\n", value, boolean->_value);
        return false;
    }

    if (strcmp(boolean->_token._literal, ((value) ? "true" : "false"))) {
        printf("boolen->_token._literal not '%s', got='%s'.\n", ((value) ? "true" : "false"), boolean->_token._literal);
        return false;
    }

    return true;
}

static bool __test_integerLiteral(const Expression *expression, int value) {
    IntegerLiteral *aux_iLiteral = ((IntegerLiteral *)expression->_ptr);

    if (aux_iLiteral->_value != value) {
        printf("Error, value not %d. got=%d\n", value, aux_iLiteral->_value);
        return false;
    }

    char buffer[sizeof(value) * 8 + 1];
    // ltoa(value, buffer, 10);
    itoa(value, buffer, 10);
    if (strcmp(aux_iLiteral->_token._literal, buffer) != 0) {
        printf("Error, _literal not %s. got=%s\n", buffer, aux_iLiteral->_token._literal);
        return false;
    }

    return true;
}

static bool __test_identifier(void) {
    const char *input = "foobar";

    Lexer lexer = new_lexer(input);
    Parser parser = new_parser(&lexer);

    Program program = program_parser(&parser);
    if (checkParserErrors(&parser)) {
        delete_data(&program, &lexer, &parser);
        return false;
    }

    if (program._statements == NULL) {
        printf("Error, program is NULL\n");
        delete_data(&program, &lexer, &parser);
        return false;
    }

    if (program._len != 1) {
        printf("Error, program._len has been 1 statements. got=%d.\n", program._len);
        delete_data(&program, &lexer, &parser);
        return false;
    }

    ExpressionStatement *aux_exprStmt = ((ExpressionStatement *)program._statements[0]._ptr);
    Identifier *aux_ident = ((Identifier *)aux_exprStmt->_expression._ptr);

    if (strcmp(aux_ident->_value, "foobar") != 0) {
        printf("Error, ident.value not %s. got=%s\n", "foobar", ((Identifier *)((ExpressionStatement *)program._statements[0]._ptr)->_expression._ptr)->_value);
        delete_data(&program, &lexer, &parser);
        return false;
    }

    delete_data(&program, &lexer, &parser);
    return true;
}

static bool __test_returnStatement(void) {
    const char *input = 
    "return 5;\n\
    return 10;\n\
    return 956345;";

    Lexer lexer = new_lexer(input);
    Parser parser = new_parser(&lexer);

    Program program = program_parser(&parser);
    if (checkParserErrors(&parser)) {
        delete_data(&program, &lexer, &parser);
        return false;
    }

    if (program._statements == NULL) {
        printf("Error, program is NULL\n");
        delete_data(&program, &lexer, &parser);
        return false;
    }

    if (program._len != 3) {
        printf("Error, program._len has been 3 statements. got=%d.\n", program._len);
        delete_data(&program, &lexer, &parser);
        return false;
    }

    for (int k=0; k < 3; k++) {
        if (program._statements[k]._type != TYPE_RETURN) {
            printf("Error, statement with index '%d' is not LET type.\n", k);
            delete_data(&program, &lexer, &parser);
            return false;
        }

        if (strcmp(((ReturnStatement *)program._statements[k]._ptr)->_token._literal, "return") != 0) {
            printf("returnStatemt._literal not 'return', got='%s'", ((ReturnStatement *)program._statements[k]._ptr)->_token._literal);
            delete_data(&program, &lexer, &parser);
            return false;
        }
    }


    delete_data(&program, &lexer, &parser);
    return true;
}

static bool __test_letStatement(void) {
    const char *input = 
    "let x = 5;\n\
    let y = 10;\n\
    let foobar = 838383;";

    Lexer lexer = new_lexer(input);
    Parser parser = new_parser(&lexer);
    
    Program program = program_parser(&parser);
    if (checkParserErrors(&parser)) {
        delete_data(&program, &lexer, &parser);
        return false;
    }

    if (program._statements == NULL) {
        printf("Error, program is NULL\n");
        delete_data(&program, &lexer, &parser);
        return false;
    }

    if (program._len != 3) {
        printf("Error, program._len has been 3 statements. got=%d.\n", program._len);
        delete_data(&program, &lexer, &parser);
        return false;
    }

    const char *tests[3] = {"x", "y", "foobar"};

    for (int k=0; k < 3; k++) {
        Statement stmt = program._statements[k];

        if (stmt._type == TYPE_LET) {
            if(!let_statement(((LetStatement *)stmt._ptr), tests[k])) {
                delete_data(&program, &lexer, &parser);
                return false;
            }
        }
        else {
            delete_data(&program, &lexer, &parser);
            printf("Error, statement with index '%d' is not LET type.\n", k);
            return false;
        }
    }

    delete_data(&program, &lexer, &parser);
    return true;
}

static void delete_data(Program *program, Lexer *lexer, Parser *parser) {
    free_program(program);
    free_lexer(lexer);
    free_parser(parser);
}

static bool let_statement(const LetStatement *const let_stmt, const char *name) {
    if (strcmp(let_stmt->_token._literal, "let") != 0) {
        printf("_literal is not 'let'. got=%s.\n", let_stmt->_token._literal);
        return false;
    }

    if (strcmp(let_stmt->_name._value, name) != 0) {
        printf("_value is not '%s'. got=%s.\n", name, let_stmt->_name._value);
        return false;
    }

    if (strcmp(let_stmt->_name._token._literal, name) != 0) {
        printf("_literal is not '%s'. got=%s.\n", name, let_stmt->_name._token._literal);
        return false;
    }

    return true;
}

static bool checkParserErrors(const Parser *const parser) {
    if (parser->error._errors == NULL) 
        return false;

    printf("parser has %d errors.\n", parser->error._len);
    for (int k=0; k < parser->error._len; k++) 
        printf(parser->error._errors[k]);

    return true;   
}
