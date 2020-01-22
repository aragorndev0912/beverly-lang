#include "ast.h"
#include "../lib/lib.h"
#include "../lib/bool.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//----------------------------------------------------------------------------------
// Constantes.
//----------------------------------------------------------------------------------
static const size_t STMT_CAP = 10;


//----------------------------------------------------------------------------------
// Struct Node.
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Struct Statement.
//----------------------------------------------------------------------------------
Statement new_stmt(void *ptr, TypeStmt type) {
    Statement stmt = {._ptr=ptr, ._type=type};
    return stmt;
}

void free_stmt(Statement *stmt) {
    if (stmt->_ptr != NULL) {
        free(stmt->_ptr);
        stmt->_ptr = NULL;
    }

    if (stmt->__string != NULL) {
        free(stmt->__string);
        stmt->__string = NULL;
    }
}

const char *string_stmt(Statement *stmt) {

    switch (stmt->_type) {
        case TYPE_LET:
            add_string(&stmt->__string, string_letStmt((LetStatement *)stmt->_ptr));
            break;

        case TYPE_RETURN:
            add_string(&stmt->__string, string_returnStmt((ReturnStatement *)stmt->_ptr));
            break;
        
        case TYPE_EXPR_STMT:
            add_string(&stmt->__string, string_exprStmt((ExpressionStatement *)stmt->_ptr));
            break;
        
        default:
            // Falta validar.
            printf("Error...\n");
            break;
    }

    return stmt->__string;
}


//----------------------------------------------------------------------------------
// Struct Expression.
//----------------------------------------------------------------------------------
void free_expression(Expression *expression) {
    if (expression->_ptr != NULL) {
        bool flag = false;
        switch (expression->_type) {
            case EXPR_IDENTIFIER:
                free_identifier((Identifier *)expression->_ptr);
                flag = true;
                break;
            
            case EXPR_INTEGER:
                free_integer_literal((IntegerLiteral *)expression->_ptr);
                flag = true;
                break;
            
            case EXPR_PREFIX:
                free_prefix_expression((PrefixExpression *)expression->_ptr);
                flag = true;
                break;
            
            case EXPR_INFIX:
                free_infix_expression((InfixExpression *)expression->_ptr);
                flag = true;
                break;
            
            case EXPR_BOOLEAN:
                free_boolean((Boolean *)expression->_ptr);
                flag = true;
                break;
            
            case EXPR_IF:
                free_if_expression((IfExpression *)expression->_ptr);
                flag = true;
                break;
            
            case EXPR_FUNCTION:
                free_function_literal((FunctionLiteral *)expression->_ptr);
                flag = true;
                break;
            
            case EXPR_CALL_FUNCTION:
                free_call_expression((CallExpression *)expression->_ptr);
                flag = true;
                break;

            default:
                break;
        }

        if (flag) {
            free(expression->_ptr);
            expression->_ptr = NULL;
        }
    }
}

const char *string_expression(Expression *expression) {
    // Falta implementar.
    if (expression->_ptr != NULL) {
        switch (expression->_type) {
            case EXPR_IDENTIFIER:
                add_string(&expression->__string, string_identifier((Identifier *)expression->_ptr));
                break;
            
            case EXPR_INTEGER:
                add_string(&expression->__string, string_integer_literal((IntegerLiteral *)expression->_ptr));
                break;
            
            case EXPR_PREFIX:
                add_string(&expression->__string, string_prefix_expression((PrefixExpression *)expression->_ptr));
                break;
            
            case EXPR_INFIX:
                add_string(&expression->__string, string_infix_expression((InfixExpression *)expression->_ptr));
                break;
            
            case EXPR_BOOLEAN:
                add_string(&expression->__string, string_boolean((Boolean *)expression->_ptr));
                break;
            
            case EXPR_IF:
                add_string(&expression->__string, string_if_expression((IfExpression *)expression->_ptr));
                break;
            
            case EXPR_FUNCTION:
                add_string(&expression->__string, string_function_literal((FunctionLiteral *)expression->_ptr));
                break;
            
            case EXPR_CALL_FUNCTION:
                add_string(&expression->__string, string_call_expression((CallExpression *)expression->_ptr));
                break;

            default:
                break;
        }
    }
    
    return (expression->__string == NULL) ? " " : expression->__string;
}


//----------------------------------------------------------------------------------
// Struct Program.
//----------------------------------------------------------------------------------
Program new_program(void) {
    Program program = (Program) {._len=0, ._cap=0, ._statements=NULL};
    return program;
}

void add_stmt_program(Program *const program, void *ptr, TypeStmt type) {
    if (program->_statements == NULL) {
        program->_cap = STMT_CAP;
        program->_statements = (Statement *) malloc(sizeof(Statement) * program->_cap);
        if (program->_statements == NULL) {
            // Falta implementar limpieza.
            printf("Error al reservar memoria para stmt program.\n");
        }
    }
    else if ((program->_len) >= program->_cap) {
        program->_cap += STMT_CAP;
        Statement *stmt_aux = (Statement *) realloc(program->_statements, sizeof(Statement) * program->_cap);
        if (stmt_aux == NULL) {
            // Falta implementar limpiza (OJO).
            printf("Error al redimencionar memoria para stmt program.\n");
        }
        program->_statements = stmt_aux;
    }
    program->_statements[program->_len++] = (Statement) {._ptr=ptr, ._type=type, .__string=NULL}; 
}

void free_program(Program *program) {
    if (program->_statements != NULL) {
        for (int k=0; k < program->_len; k++)  {

            switch (program->_statements[k]._type) {
                case TYPE_LET:
                    free_letStmt((LetStatement *)program->_statements[k]._ptr);
                    free_stmt(&program->_statements[k]);
                    break;
                
                case TYPE_RETURN:
                    free_returnStmt((ReturnStatement *)program->_statements[k]._ptr);
                    free_stmt(&program->_statements[k]);
                    break;
                
                case TYPE_EXPR_STMT:
                    free_exprStmt((ExpressionStatement *)program->_statements[k]._ptr);
                    free_stmt(&program->_statements[k]);
                    break;

                default:
                    printf("Error to delete --> %d...\n", program->_statements[k]._type); // Falta implementar.
            }
        }
    
        program->_statements = NULL;
    }

    if (program->__string != NULL) {
        free(program->__string);
        program->__string = NULL;
    }
}

const char *string_program(Program *program) {
    if (program->_statements != NULL) {
        for (size_t k=0; k < program->_len; k++) {
            switch (program->_statements[k]._type) {
                case TYPE_LET:
                    add_string(&program->__string, string_letStmt(((LetStatement *)program->_statements[k]._ptr)));
                    break;
                
                case TYPE_RETURN:
                    add_string(&program->__string, string_returnStmt(((ReturnStatement *)program->_statements[k]._ptr)));
                    break;

                case TYPE_EXPR_STMT:
                    add_string(&program->__string, string_exprStmt(((ExpressionStatement *)program->_statements[k]._ptr)));
                    break;

                default:
                    // pass
                    continue;
            }
        }
    }

    return (program->__string == NULL) ? " ": program->__string;
}


//----------------------------------------------------------------------------------
// Struct Identifier.
//----------------------------------------------------------------------------------
Identifier new_identifier(const Token *const token, const char *value) {
    Identifier ident = (Identifier) {0};
    ident._token = new_token(token->_type, copy_string(token->_literal));
    ident._value = copy_string(value);

    return ident;
}

void free_identifier(Identifier *ident) {
    if (ident != NULL) {
        free_token(&ident->_token);

        // Libero la memoria reservada de la veriable _value.
        if (ident->_value != NULL) {
            free(ident->_value);
            ident->_value = NULL;
        }
    }
}

const char *string_identifier(const Identifier *const ident) {
    return ident->_value;
}

//----------------------------------------------------------------------------------
// Struct LetStatement.
//----------------------------------------------------------------------------------
LetStatement new_letStmt(void) {
    LetStatement let_statement = (LetStatement){0};
    let_statement._token = (Token){._type=BEV_UNDEFINED, ._literal=NULL};
    let_statement._name = (Identifier){
        ._token=(Token){._type=BEV_UNDEFINED, ._literal=NULL},
        ._value=NULL  
    };

    let_statement._value = (Expression){
        ._ptr=NULL,
        ._type=EXPR_FAILURE,
        .__string=NULL,
    };

    let_statement.__string = NULL;

    return let_statement;
}


const char *string_letStmt(LetStatement *let_stmt) {
    add_string(&let_stmt->__string, let_stmt->_token._literal);
    add_string(&let_stmt->__string, " ");
    add_string(&let_stmt->__string, string_identifier(&let_stmt->_name));
    add_string(&let_stmt->__string, " = ");
    add_string(&let_stmt->__string, string_expression(&let_stmt->_value));

    return let_stmt->__string;
}

void free_letStmt(LetStatement *let_stmt) {
    free_identifier(&let_stmt->_name);
    free_token(&let_stmt->_token);
    // free_expression(); falta por hacer.
    if (let_stmt->__string != NULL) {
        free(let_stmt->__string);
        let_stmt->__string = NULL;
    }

    if (let_stmt->_value._ptr != NULL)
        free_expression(&let_stmt->_value);
}


//----------------------------------------------------------------------------------
// Struct ReturnStatement.
//----------------------------------------------------------------------------------
ReturnStatement new_returnStmt(void) {
    ReturnStatement return_statement = (ReturnStatement){0};
    return_statement._token = (Token){._type=BEV_UNDEFINED, ._literal=NULL};
    return_statement._value = (Expression){._ptr=NULL, ._type=EXPR_FAILURE, .__string=NULL,};
    return_statement.__string = NULL;

    return return_statement;
}

const char *string_returnStmt(ReturnStatement *return_stmt) {
    add_string(&return_stmt->__string, return_stmt->_token._literal);
    add_string(&return_stmt->__string, " ");
    add_string(&return_stmt->__string, string_expression(&return_stmt->_value));
    add_string(&return_stmt->__string, ";");

    return (return_stmt->__string == NULL) ? " ": return_stmt->__string;
}

void free_returnStmt(ReturnStatement *return_stmt) {
    free_token(&return_stmt->_token);
    if (return_stmt->__string != NULL) {
        free(return_stmt->__string);
        return_stmt->__string = NULL;
    }

    if (return_stmt->_value._ptr != NULL)
        free_expression(&return_stmt->_value);
}


//----------------------------------------------------------------------------------
// Struct ExpressionStatement.
//----------------------------------------------------------------------------------
ExpressionStatement new_exprStmt(void) {
    ExpressionStatement expression_statement = (ExpressionStatement){0};
    expression_statement._token = (Token){._type=BEV_UNDEFINED, ._literal=NULL};
    expression_statement._expression = (Expression){._ptr=NULL, ._type=EXPR_FAILURE, .__string=NULL,};
    expression_statement.__string = NULL;

    return expression_statement;
}

const char *string_exprStmt(ExpressionStatement *expr_stmt) {
    add_string(&expr_stmt->__string, string_expression(&expr_stmt->_expression));
    return expr_stmt->__string;
}

void free_exprStmt(ExpressionStatement *expr_stmt) {
    free_expression(&expr_stmt->_expression);
    free_token(&expr_stmt->_token);

    if (expr_stmt->__string != NULL) {
        free(expr_stmt->__string);
        expr_stmt->__string = NULL;
    }
}


//----------------------------------------------------------------------------------
// struct IntegerLiteral.
//----------------------------------------------------------------------------------
IntegerLiteral new_integer_literal(const Token *token, int value) {
    IntegerLiteral int_literal = (IntegerLiteral) {0};
    int_literal._token = new_token(token->_type, copy_string(token->_literal));
    int_literal._value = value;

    return int_literal;
}

const char *string_integer_literal(const IntegerLiteral *int_literal) {
    return int_literal->_token._literal;
}

void free_integer_literal(IntegerLiteral *int_literal) {
    free_token(&int_literal->_token);
}


//----------------------------------------------------------------------------------
// struct PrefixExpression.
//----------------------------------------------------------------------------------
const char *string_prefix_expression(PrefixExpression *prefix_expression) {
    add_string(&prefix_expression->__string, "(");
    add_string(&prefix_expression->__string, prefix_expression->_operator);
    add_string(&prefix_expression->__string, string_expression(&prefix_expression->_right));
    add_string(&prefix_expression->__string, ")");
    
    return prefix_expression->__string;
}

void free_prefix_expression(PrefixExpression *prefix_expression) {
    if (prefix_expression->_operator != NULL) {
        free(prefix_expression->_operator);
        prefix_expression->_operator = NULL;
    }

    free_token(&prefix_expression->_token);
    free_expression(&prefix_expression->_right);

    if (prefix_expression->__string != NULL) {
        free(prefix_expression->__string);
        prefix_expression->__string = NULL;
    }
}

//----------------------------------------------------------------------------------
// struct InfixExpression.
//----------------------------------------------------------------------------------
const char *string_infix_expression(InfixExpression *infix_expression) {
    add_string(&infix_expression->__string, "(");
    add_string(&infix_expression->__string, string_expression(&infix_expression->_left));
    add_string(&infix_expression->__string, " ");
    add_string(&infix_expression->__string, infix_expression->_operator);
    add_string(&infix_expression->__string, " ");
    add_string(&infix_expression->__string, string_expression(&infix_expression->_right));
    add_string(&infix_expression->__string, ")");

    return infix_expression->__string;
}

void free_infix_expression(InfixExpression *infix_expression) {
    if (infix_expression != NULL) {
        free_token(&infix_expression->_token);
        free_expression(&infix_expression->_left);
        free_expression(&infix_expression->_right);

        if (infix_expression->_operator != NULL) {
            free(infix_expression->_operator);
            infix_expression->_operator = NULL;
        }

        if (infix_expression->__string != NULL) {
            free(infix_expression->__string);
            infix_expression->__string = NULL;
        }
    }
}

//----------------------------------------------------------------------------------
// struct Boolean.
//----------------------------------------------------------------------------------
const char *string_boolean(Boolean *boolean) {
    return boolean->_token._literal;
}

void free_boolean(Boolean *boolean) {
    if (boolean != NULL) {
        free_token(&boolean->_token);
    }
}


//----------------------------------------------------------------------------------
// struct BlockStatement.
//----------------------------------------------------------------------------------

void free_block_statement(BlockStatement *block_statement) {
    free_token(&block_statement->_token);
    if (block_statement->_statements != NULL) {
        for (size_t k=0; k < block_statement->_len; k++)  {
            switch (block_statement->_statements[k]._type) {
                case TYPE_LET:
                    free_letStmt((LetStatement *)block_statement->_statements[k]._ptr);
                    free_stmt(&block_statement->_statements[k]);
                    break;
                
                case TYPE_RETURN:
                    free_returnStmt((ReturnStatement *)block_statement->_statements[k]._ptr);
                    free_stmt(&block_statement->_statements[k]);
                    break;
                
                case TYPE_EXPR_STMT:
                    free_exprStmt((ExpressionStatement *)block_statement->_statements[k]._ptr);
                    free_stmt(&block_statement->_statements[k]);
                    break;

                default:
                    printf("(free block)Error to delete --> ... %d, k=%d\n", block_statement->_statements[k]._type, k); // Falta implementar.
            }
        }

        free(block_statement->_statements);
        block_statement->_statements = NULL;
    }

    if (block_statement->__string != NULL) {
        free(block_statement);
        block_statement = NULL;
    }
}

const char *string_block_statement(BlockStatement *block_statement) {

    for (size_t k=0; k < block_statement->_len; k++)
        add_string(&block_statement->__string, string_stmt(&block_statement->_statements[k]));

    return block_statement->__string;
}

static const size_t SIZE_BLOCK = 10;

void add_element_block_statement(BlockStatement *block_statement, Statement statement) {
    if (block_statement->_statements == NULL) {
        block_statement->_len = 0;
        block_statement->_cap = SIZE_BLOCK;
        block_statement->_statements = (Statement *) malloc(sizeof(Statement) * block_statement->_cap);
        if (block_statement->_statements == NULL) {
            // Falta implementar.
            printf("Error al reservar memoria statement block.\n");
        }
        block_statement->_statements[block_statement->_len++] = (Statement) {._ptr=statement._ptr, ._type=statement._type, .__string=statement.__string};
    }
    else {
        if (block_statement->_len >= block_statement->_cap) {
            block_statement->_cap += SIZE_BLOCK;
            Statement *aux = (Statement *) realloc(block_statement->_statements, block_statement->_cap);
            if (aux == NULL) {
                // falta implementar.
                printf("Error al redimensionar statement block.\n");
            }

            block_statement->_statements = aux;
        }

        block_statement->_statements[block_statement->_len++] = (Statement) {._ptr=statement._ptr, ._type=statement._type, .__string=statement.__string};
    }
}


//----------------------------------------------------------------------------------
// struct IfExpression.
//----------------------------------------------------------------------------------
void free_if_expression(IfExpression *if_expression) {
    free_token(&if_expression->_token);
    free_expression(&if_expression->_condition);
    free_block_statement(&if_expression->_if_consequence);
    free_block_statement(&if_expression->_else_consequence);

    if (if_expression->__string != NULL) {
        free(if_expression->__string);
        if_expression->__string = NULL;
    }
}

const char *string_if_expression(IfExpression *if_expression) {
    add_string(&if_expression->__string, "if");

    add_string(&if_expression->__string, string_expression(&if_expression->_condition));
    add_string(&if_expression->__string, " ");
    add_string(&if_expression->__string, string_block_statement(&if_expression->_if_consequence));

    if (if_expression->_else_consequence._statements != NULL) {
        add_string(&if_expression->__string, " ");
        add_string(&if_expression->__string, string_block_statement(&if_expression->_else_consequence)); 
    }

    return if_expression->__string;
}


//----------------------------------------------------------------------------------
// struct FunctionLiteral.
//----------------------------------------------------------------------------------

static const size_t SIZE_PARAMETERS = 10;

void add_parameter(Parameter *parameter, Identifier identifier) {
    if (parameter->_identifiers == NULL) {
        parameter->_cap = SIZE_PARAMETERS;
        parameter->_len = 0;
        parameter->_identifiers = (Identifier *) malloc(sizeof(Identifier) * parameter->_cap);
        if (parameter->_identifiers == NULL) {
            // Falta implementacion.
            printf("Error al reservar memoria en la estructura Parameter.\n");
            return;
        }
    }
    else if (parameter->_len >= parameter->_cap) {
        parameter->_cap += SIZE_PARAMETERS;
        Identifier *aux = (Identifier *) realloc(parameter->_identifiers, parameter->_cap);
        if (aux == NULL) {
            printf("Errpr al redimensionar la memoria de la estructura Parameter.\n");
            return;
        }
        parameter->_identifiers = aux;
        aux = NULL;
    }
    parameter->_identifiers[parameter->_len++] = (Identifier) {._token=identifier._token, ._value=identifier._value};
}

void free_parameter(Parameter *parameter) {
    if (parameter->_identifiers != NULL) {
        for (size_t k=0; k < parameter->_len; k++) 
            free_identifier(&parameter->_identifiers[k]);

        free(parameter->_identifiers);
        parameter->_identifiers = NULL;
    }
}

const char *string_function_literal(FunctionLiteral *function){
    
    add_string(&function->__strings, function->_token._literal);
    add_string(&function->__strings, "(");
    for (size_t k=0; k < function->_parameters._len; k++) {
        add_string(&function->__strings, string_identifier(&function->_parameters._identifiers[k]));
        if (k < (function->_parameters._len - 1)) 
            add_string(&function->__strings, ", ");
    }
    add_string(&function->__strings, ")");
    add_string(&function->__strings, string_block_statement(&function->_block));
    return function->__strings;
}

void free_function_literal(FunctionLiteral *function){
    free_token(&function->_token);
    free_parameter(&function->_parameters);
    free_block_statement(&function->_block);
    if (function->__strings != NULL) {
        free(function->__strings);
        function->__strings = NULL;
    }
}


//----------------------------------------------------------------------------------
// struct CallExpression.
//----------------------------------------------------------------------------------

static const size_t SIZE_ARGUMENT_CALL = 10;

bool add_argument(Argument *argument, Expression expression) {
    if (argument->_expressions == NULL) {
        argument->_len = 0;
        argument->_cap = SIZE_ARGUMENT_CALL;

        argument->_expressions = (Expression *) malloc(sizeof(Expression) * argument->_cap);
        if (argument->_expressions == NULL) 
            return false;
    }
    else if (argument->_len >= argument->_cap) {
        argument->_cap += SIZE_ARGUMENT_CALL;
        Expression *aux = (Expression *) realloc(argument->_expressions, argument->_cap);
        if (aux == NULL) 
            return false;
            
        argument->_expressions = aux;
    }
    argument->_expressions[argument->_len++] = (Expression) {._ptr=expression._ptr, ._type=expression._type, .__string=expression.__string};

    return true;
}

void free_argument(Argument *argument) {
    if (argument->_expressions != NULL) {
        for (size_t k=0; k < argument->_len; k++) 
            free_expression(&argument->_expressions[k]);

        free(argument->_expressions);
        argument->_expressions = NULL;
    }
}

const char *string_call_expression(CallExpression *call) {
    add_string(&call->__string, string_expression(&call->_function));
    add_string(&call->__string, "(");
    for (size_t k=0; k < call->_arguments._len; k++) {
        add_string(&call->__string, string_expression(&call->_arguments._expressions[k]));
        if (k < (call->_arguments._len - 1))
            add_string(&call->__string, ", ");
    }
    add_string(&call->__string, ")");
    return call->__string;
}

void free_call_expression(CallExpression *call) {
    free_token(&call->_token);
    free_expression(&call->_function);
    free_argument(&call->_arguments);
    if (call->__string != NULL) {
        free(call->__string);
        call->__string = NULL;
    }
}