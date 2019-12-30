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

} ExpressionType;

typedef struct Expression {
    void *_ptr;
    ExpressionType _type;

} Expression;

void free_expression(Expression *expression);

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

} ExpressionStatement;

const char *string_exprStmt(ExpressionStatement *expr_stmt);

void free_exprStmt(ExpressionStatement *expr_stmt);

#endif //_AST_H