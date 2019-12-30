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
    if (program->_statements != NULL) {
        for (int k=0; k < program->_len; k++)  {

            switch (program->_statements[k]._type) {
                case TYPE_LET:
                    free_letStmt(((LetStatement *)program->_statements[k]._ptr));
                    break;
                
                case TYPE_RETURN:
                    free_returnStmt(((ReturnStatement *)program->_statements[k]._ptr));
                    break;

                default:
                    printf("Error to delete...\n"); // Falta implementar.
            }

            free_stmt(&program->_statements[k]);
        }
    
        program->_statements = NULL;
    }

    if (program->__string != NULL) {
        free(program->__string);
        program->__string = NULL;
    }
}

const char *string_program(Program *program) {
    if (program->_statements != NULL) {
        for (size_t k=0; k < program->_len; k++) {
            switch (program->_statements[k]._type) {
                case TYPE_LET:
                    add_string(&program->__string, string_letStmt(((LetStatement *)program->_statements[k]._ptr)));
                    break;
                
                case TYPE_RETURN:
                    add_string(&program->__string, string_returnStmt(((ReturnStatement *)program->_statements[k]._ptr)));
                    break;

                default:
                    continue;
            }
        }
    }

    return (program->__string == NULL) ? " ": program->__string;
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
    add_string(&let_stmt->__string, let_stmt->_token._literal);
    add_string(&let_stmt->__string, " ");
    add_string(&let_stmt->__string, string_identifier(&let_stmt->_name));
    add_string(&let_stmt->__string, " = ");

    return (let_stmt->__string == NULL) ? ("") : (let_stmt->__string);
}

void free_letStmt(LetStatement *let_stmt) {
    free_identifier(&let_stmt->_name);
    free_token(&let_stmt->_token);
    // free_expression(); falta por hacer.
    if (let_stmt->__string != NULL) {
        free(let_stmt->__string);
        let_stmt->__string = NULL;
    }
}


//----------------------------------------------------------------------------------
// Struct ReturnStatement.
//----------------------------------------------------------------------------------
const char *string_returnStmt(ReturnStatement *return_stmt) {
    add_string(&return_stmt->__string, return_stmt->_token._literal);
    add_string(&return_stmt->__string, " ");
    // Falta implementacion.
    // if (return_stmt->_value != NULL) {
    // }

    add_string(&return_stmt->__string, ";");

    return (return_stmt->__string == NULL) ? " ": return_stmt->__string;
}

void free_returnStmt(ReturnStatement *return_stmt) {
    free_token(&return_stmt->_token);
    if (return_stmt->__string != NULL) {
        free(return_stmt->__string);
        return_stmt->__string = NULL;
    }
}


//----------------------------------------------------------------------------------
// Struct ExpressionStatement.
//----------------------------------------------------------------------------------
const char *string_exprStmt(ExpressionStatement *expr_stmt) {
    // Falta implementacion.
    // if (expr_stmt->_expression != NULL) {
    // }

    return " ";
}