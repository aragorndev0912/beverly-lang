#include "ast.h"
#include "../lib/lib.h"
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
}


//----------------------------------------------------------------------------------
// Struct Expression.
//----------------------------------------------------------------------------------

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
        }
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
}

const char *string_program(Program *program) {
    // Falta implementar.
    return NULL;
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
const char *string_letStmt(LetStatement *let_stmt) {
    // Falta implementar.
    return NULL;
}


//----------------------------------------------------------------------------------
// Struct ReturnStatement.
//----------------------------------------------------------------------------------
const char *string_returnStmt(ReturnStatement *return_stmt) {
    // Falta implementar.
    return NULL;
}


//----------------------------------------------------------------------------------
// Struct ExpressionStatement.
//----------------------------------------------------------------------------------
const char *string_exprStmt(ExprStatement *expr_stmt) {
    // Falta implementar cuando expression != NULL.
    return NULL;
}
