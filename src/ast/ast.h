#ifndef _AST_H
#define _AST_H

#include <stddef.h>
#include "../token/token.h"

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

} Statement;

Statement new_stmt(void *ptr, TypeStmt type);

void free_stmt(Statement *stmt);


//----------------------------------------------------------------------------------
// struct Expression.
//----------------------------------------------------------------------------------
typedef enum ExpressionType {
    EXPR_FAILURE = 0,
    EXPR_IDENTIFIER,
    EXPR_INTEGER,
    EXPR_PREFIX,
    EXPR_INFIX,

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

const char *string_exprStmt(ExpressionStatement *expr_stmt);

void free_exprStmt(ExpressionStatement *expr_stmt);


//----------------------------------------------------------------------------------
// struct IntegerLiteral.
//----------------------------------------------------------------------------------
typedef struct IntegerLiteral {
    Token _token;
    long long _value; // 8 bytes

} IntegerLiteral;

IntegerLiteral new_integer_literal(const Token *token, long long value);

const char *string_integer_literal(const IntegerLiteral *intLiteral);

void free_integer_literal(IntegerLiteral *IntegerLiteral);


//----------------------------------------------------------------------------------
// struct PrefixExpression.
//----------------------------------------------------------------------------------
typedef struct PrefixExpression {
    Token _token;
    char *_operator;
    Expression _right;

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

#endif //_AST_H