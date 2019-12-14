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
    LET = 0,

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

Program new_program(size_t cap);

void add_stmt_program(Program *const program, void *ptr, TypeStmt type);

void free_program(Program *program);


//----------------------------------------------------------------------------------
// Expression Identifier.
//----------------------------------------------------------------------------------
typedef struct Identifier {
    Token _token;
    char *_value;

} Identifier;

void node_expression_identifier(Identifier *identifier);

const char *token_literal_identifier(Identifier *identifier);

void free_identifier(Identifier *identifier);


//----------------------------------------------------------------------------------
// Expression LetStatement.
//----------------------------------------------------------------------------------
typedef struct LetStatement {
    Token _token;
    Identifier _name;
    Expression _value;

} LetStatement;

Statement new_letstmt(const Token *const token);

#endif //_AST_H