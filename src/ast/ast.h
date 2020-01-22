#ifndef _AST_H
#define _AST_H

#include <stddef.h>
#include "../token/token.h"
#include "../lib/bool.h"

//----------------------------------------------------------------------------------
// struct Node.
//----------------------------------------------------------------------------------
typedef struct Node {
    const char *_literal;

} Node;

const char *token_literal_node(const Node *const node);

void free_node(Node *node);


//----------------------------------------------------------------------------------
// struct Statement.
//----------------------------------------------------------------------------------
typedef enum TypeStmt {
    TYPE_FAILURE = 0,
    TYPE_LET,
    TYPE_RETURN,
    TYPE_EXPR_STMT,

} TypeStmt;

typedef struct Statement {
    void *_ptr;
    TypeStmt _type;

    char *__string;

} Statement;

Statement new_stmt(void *ptr, TypeStmt type);

void free_stmt(Statement *stmt);

const char *string_stmt(Statement *stmt);

//----------------------------------------------------------------------------------
// struct Expression.
//----------------------------------------------------------------------------------
typedef enum ExpressionType {
    EXPR_FAILURE = 0,
    EXPR_VOID,
    EXPR_IDENTIFIER,
    EXPR_INTEGER,
    EXPR_PREFIX,
    EXPR_INFIX,
    EXPR_BOOLEAN,
    EXPR_IF,
    EXPR_FUNCTION,
    EXPR_CALL_FUNCTION,

} ExpressionType;

typedef struct Expression {
    void *_ptr;
    ExpressionType _type;

    char *__string;

} Expression;

void free_expression(Expression *expression);

const char *string_expression(Expression *expression);

//----------------------------------------------------------------------------------
// struct Program.
//----------------------------------------------------------------------------------
typedef struct Program {
    Statement *_statements;
    size_t _len;
    size_t _cap;

    char *__string;

} Program;

Program new_program(void);

void add_stmt_program(Program *const program, void *ptr, TypeStmt type);

void free_program(Program *program);

const char *string_program(Program *program);


//----------------------------------------------------------------------------------
// struct Identifier.
//----------------------------------------------------------------------------------
typedef struct Identifier {
    Token _token;
    char *_value;

} Identifier;

Identifier new_identifier(const Token *const token, const char *value);

void free_identifier(Identifier *ident);

const char *string_identifier(const Identifier *const ident);


//----------------------------------------------------------------------------------
// struct LetStatement.
//----------------------------------------------------------------------------------
typedef struct LetStatement {
    Token _token;
    Identifier _name;
    Expression _value;

    char *__string;

} LetStatement;

LetStatement new_letStmt(void);

const char *string_letStmt(LetStatement *let_stmt);

void free_letStmt(LetStatement *let_stmt);

//----------------------------------------------------------------------------------
// struct ReturnStatement.
//----------------------------------------------------------------------------------
typedef struct ReturnStatement {
    Token _token;
    Expression _value;

    char *__string;

} ReturnStatement;

ReturnStatement new_returnStmt(void);

const char *string_returnStmt(ReturnStatement *return_stmt);

void free_returnStmt(ReturnStatement *return_stmt);


//----------------------------------------------------------------------------------
// struct ExpressionStatement.
//----------------------------------------------------------------------------------
typedef struct ExpressionStatement {
    Token _token;
    Expression _expression;

    char *__string;

} ExpressionStatement;

ExpressionStatement new_exprStmt(void);

const char *string_exprStmt(ExpressionStatement *expr_stmt);

void free_exprStmt(ExpressionStatement *expr_stmt);


//----------------------------------------------------------------------------------
// struct IntegerLiteral.
//----------------------------------------------------------------------------------
typedef struct IntegerLiteral {
    Token _token;
    int _value; // 8 bytes

} IntegerLiteral;

IntegerLiteral new_integer_literal(const Token *token, int value);

const char *string_integer_literal(const IntegerLiteral *intLiteral);

void free_integer_literal(IntegerLiteral *IntegerLiteral);


//----------------------------------------------------------------------------------
// struct PrefixExpression.
//----------------------------------------------------------------------------------
typedef struct PrefixExpression {
    Token _token;
    char *_operator;
    Expression _right;

    char *__string;

} PrefixExpression;

const char *string_prefix_expression(PrefixExpression *prefix_expression);

void free_prefix_expression(PrefixExpression *prefix_expression);



//----------------------------------------------------------------------------------
// struct InfixExpression.
//----------------------------------------------------------------------------------
typedef struct InfixExpression {
    Token _token;
    char *_operator;
    Expression _left;
    Expression _right;

    char *__string;

} InfixExpression;

const char *string_infix_expression(InfixExpression *infix_expression);

void free_infix_expression(InfixExpression *infix_expression);


//----------------------------------------------------------------------------------
// struct Boolean.
//----------------------------------------------------------------------------------
typedef struct Boolean {
    Token _token;
    bool _value;

} Boolean;

const char *string_boolean(Boolean *boolean);

void free_boolean(Boolean *boolean);


//----------------------------------------------------------------------------------
// struct BlockStatement.
//----------------------------------------------------------------------------------
typedef struct BlockStatement {
    Token _token;
    Statement *_statements;
    size_t _len;
    size_t _cap;

    char *__string;

} BlockStatement;

void free_block_statement(BlockStatement *block_statement);

const char *string_block_statement(BlockStatement *block_statement);

void add_element_block_statement(BlockStatement *block_statement, Statement statement);

//----------------------------------------------------------------------------------
// struct IfExpression.
//----------------------------------------------------------------------------------
typedef struct IfExpression {
    Token _token;
    Expression _condition;
    BlockStatement _if_consequence;
    BlockStatement _else_consequence;

    char *__string;

} IfExpression;

void free_if_expression(IfExpression *if_expression);

const char *string_if_expression(IfExpression *if_expression);


//----------------------------------------------------------------------------------
// struct FunctionLiteral.
//----------------------------------------------------------------------------------
typedef struct Parameter {
    Identifier *_identifiers;
    size_t _len;
    size_t _cap;

} Parameter;

void add_parameter(Parameter *parameter, Identifier identifier);

void free_parameter(Parameter *parameter);

typedef struct FunctionLiteral {
    Token _token;
    Parameter _parameters;
    BlockStatement _block;

    char *__strings;

} FunctionLiteral;

const char *string_function_literal(FunctionLiteral *function);

void free_function_literal(FunctionLiteral *function);


//----------------------------------------------------------------------------------
// struct CallExpression.
//----------------------------------------------------------------------------------
typedef struct Argument {
    Expression *_expressions;
    size_t _len;
    size_t _cap;

} Argument;

bool add_argument(Argument *argument, Expression expression);

void free_argument(Argument *argument);

typedef struct CallExpression {
    Token _token;
    Expression _function;
    Argument _arguments;

    char *__string;

} CallExpression;

const char *string_call_expression(CallExpression *call);

void free_call_expression(CallExpression *call);

#endif //_AST_H