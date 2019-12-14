#include "ast.h"
#include "../lib/lib.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
Identifier new_identifier(const Token *const token, const char *value) {
    Identifier ident = (Identifier) {0};
    
    ident._token = new_token(token->_type, copy_string(token->_literal));
    ident._value = copy_string(value);

    return ident;
}

void free_identifier(Identifier *ident) {
    if (ident != NULL) {
        free_token(&ident->_token);
        free(ident->_value);
        ident->_value = NULL;
    }
}

//----------------------------------------------------------------------------------
// Struct Program.
//----------------------------------------------------------------------------------

static const size_t STMT_CAP = 10;

Program new_program(size_t cap) {
    Program program = (Program) {._len=0, ._cap=cap, ._statements=NULL};
    return program;
}

void add_stmt_program(Program *const program, void *ptr, TypeStmt type) {
    if (program->_statements == NULL) {
        program->_statements = (Statement *) malloc(sizeof(Statement) * program->_cap);
    }
    else if ((program->_len) >= program->_cap) {
        program->_cap += STMT_CAP;
        Statement *stmt_aux = (Statement *) realloc(program->_statements, sizeof(Statement) * program->_cap);
        if (stmt_aux == NULL) {
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
    
    program->_statements = NULL;
    printf("PROGRAM DELETED.\n");
}

//----------------------------------------------------------------------------------
// Struct LetStatement.
//----------------------------------------------------------------------------------
