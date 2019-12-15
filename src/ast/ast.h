#ifndef _AST_H
#define _AST_H

#include <stddef.h>
#include "../token/token.h"

//----------------------------------------------------------------------------------
// Node definitions.
//----------------------------------------------------------------------------------
// Struct type Node.
typedef struct Node {
    const char *_literal;

} Node;

const char *token_literal_node(const Node *const node);

void free_node(Node *node);


//----------------------------------------------------------------------------------
// Statement declarations.
//----------------------------------------------------------------------------------
typedef enum TypeStmt {
    TYPE_FAILURE = 0,
    TYPE_LET,
    TYPE_RETURN,

} TypeStmt;

typedef struct Statement {
    void *_ptr;
    TypeStmt _type;

} Statement;

Statement new_stmt(void *ptr, TypeStmt type);

void free_stmt(Statement *stmt);

//----------------------------------------------------------------------------------
// Expression declarations.
//----------------------------------------------------------------------------------
typedef struct Expression {
    Node _node;

} Expression;

void node_expression(const Expression *const expression);

void free_expression(Expression *expression);


//----------------------------------------------------------------------------------
// Expression Program.
//----------------------------------------------------------------------------------
typedef struct Program {
    Statement *_statements;
    size_t _len;
    size_t _cap;

} Program;

Program new_program(void);

void add_stmt_program(Program *const program, void *ptr, TypeStmt type);

void free_program(Program *program);


//----------------------------------------------------------------------------------
// Expression Identifier.
//----------------------------------------------------------------------------------
typedef struct Identifier {
    Token _token;
    char *_value;

} Identifier;

Identifier new_identifier(const Token *const token, const char *value);

void free_identifier(Identifier *ident);

//----------------------------------------------------------------------------------
// Expression LetStatement.
//----------------------------------------------------------------------------------
typedef struct LetStatement {
    Token _token;
    Identifier _name;
    Expression _value;

} LetStatement;

//----------------------------------------------------------------------------------
// Return Statement.
//----------------------------------------------------------------------------------

typedef struct ReturnStatement {
    Token _token;
    Expression _value;

} ReturnStatement;

#endif //_AST_H