#include "ast.h"
#include <stdlib.h>

//----------------------------------------------------------------------------------
// Struct Node.
//----------------------------------------------------------------------------------
const char *token_literal_node(const Node *const node) {
    return node->_literal;
}

void free_node(Node *node) {
    // Falta implementar.
}

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
}

//----------------------------------------------------------------------------------
// Struct Expression.
//----------------------------------------------------------------------------------
void node_expression(const Expression *const expression) {
    // Falta implementar.
}

void free_expression(Expression *expression) {
    // Falta implementar.
}

//----------------------------------------------------------------------------------
// Struct Identifier.
//----------------------------------------------------------------------------------
void node_expression_identifier(Identifier *identifier) {
    // Falta implementar.
}

const char *token_literal_identifier(Identifier *identifier) {
    return identifier->_token._literal;
}

void free_identifier(Identifier *identifier) {
    // Falta implementar.
}

//----------------------------------------------------------------------------------
// Struct Program.
//----------------------------------------------------------------------------------
Program new_program(size_t cap) {
    Program program = {._len=0, ._cap=cap};
    program._statements = (Statement *) malloc(sizeof(Statement) * cap);

    return program;
}

const char *token_literal_program(Program *program) {
    // Falta implementar.
    return NULL;
}

void free_program(Program *program) {
    for (int k=0; k < program->_len; k++) 
        free_stmt(&program->_statements[k]);
}

//----------------------------------------------------------------------------------
// Struct LetStatement.
//----------------------------------------------------------------------------------
void node_statement_letStatement(LetStatement *let_statement) {
    // Falta implementar.
}

const char *token_literal_letStatement(LetStatement *let_statement) {
    return let_statement->_token._literal;
}

void free_letStatement(LetStatement *let_statement) {
    // Falta implementar.
}