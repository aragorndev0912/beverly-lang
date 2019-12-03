#ifndef _AST_H
#define _AST_H

#include <stddef.h>
#include "token.h"

//----------------------------------------------------------------------------------
// Node definitions.
//----------------------------------------------------------------------------------
// Struct type Node.
typedef struct Node {
    const char *literal;

} Node;

const char *token_literal_node(const Node *const node);

void free_node(Node *node);


//----------------------------------------------------------------------------------
// Statement declarations.
//----------------------------------------------------------------------------------
// Struct type Statement.
typedef struct Statement {
    Node node;

} Statement;

void node_statement(const Statement *const statement);

void free_statement(Statement *statement);


//----------------------------------------------------------------------------------
// Expression declarations.
//----------------------------------------------------------------------------------
typedef struct Expression {
    Node node;

} Expression;

void node_expression(const Expression *const expression);

void free_expression(Expression *expression);


//----------------------------------------------------------------------------------
// Expression Program.
//----------------------------------------------------------------------------------
typedef struct Program {
    Statement *statements;
    size_t len;
    size_t cap;

} Program;

const char *token_literal_program(Program *program);

void free_program(Program *program);


//----------------------------------------------------------------------------------
// Expression Identifier.
//----------------------------------------------------------------------------------
typedef struct Identifier {
    Token token;
    char *value;

} Identifier;

void node_expression_identifier(Identifier *identifier);

const char *token_literal_identifier(Identifier *identifier);

void free_identifier(Identifier *identifier);


//----------------------------------------------------------------------------------
// Expression LetStatement.
//----------------------------------------------------------------------------------
typedef struct LetStatement {
    Token token;
    Identifier name;
    Expression value;

} LetStatement;

void node_statement_letStatement(LetStatement *let_statement);

const char *token_literal_letStatement(LetStatement *let_statement);

void free_letStatement(LetStatement *let_statement);

#endif //_AST_H