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

static const size_t STMT_CAP = 10;

Program new_program(size_t cap) {
    Program program = {._len=0, ._cap=cap, ._statements=NULL};

    return program;
}

void add_stmt_program(Program *const program, void *ptr, TypeStmt type) {
    if (program->_statements == NULL) {
        program->_statements = (Statement *) malloc(sizeof(Statement) * program->_cap);
    }
    else if ((program->_len) >= program->_cap) {
        program->_cap += STMT_CAP;
        Statement *stmt_aux = (Statement *) realloc(program->_statements, sizeof(Statement) * program->_cap);
        if (stmt_aux) {
            // Falta implementar limpiza (OJO).
        }
        program->_statements = stmt_aux;
    }

    program->_statements[program->_len]._ptr = ptr;
    program->_statements[program->_len]._type = type;
    program->_len += 1;
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