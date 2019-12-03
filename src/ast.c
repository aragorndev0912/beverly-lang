#include "ast.h"

//----------------------------------------------------------------------------------
// Struct Node.
//----------------------------------------------------------------------------------
const char *token_literal_node(const Node *const node) {
    return node->literal;
}

void free_node(Node *node) {
    // Falta implementar.
}

//----------------------------------------------------------------------------------
// Struct Statement.
//----------------------------------------------------------------------------------
void node_statement(const Statement *const statement) {
    // Falta implementar.
}

void free_statement(Statement *statement) {
    // Falta implementar.
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
    return identifier->token._literal;
}

void free_identifier(Identifier *identifier) {
    // Falta implementar.
}

//----------------------------------------------------------------------------------
// Struct Program.
//----------------------------------------------------------------------------------
const char *token_literal_program(Program *program) {
    if (program->len > 0)
        return token_literal_node(&(program->statements[0].node));
    
    return "";
}

void free_program(Program *program) {
    // Falta implementar.
}

//----------------------------------------------------------------------------------
// Struct LetStatement.
//----------------------------------------------------------------------------------
void node_statement_letStatement(LetStatement *let_statement) {
    // Falta implementar.
}

const char *token_literal_letStatement(LetStatement *let_statement) {
    return let_statement->token._literal;
}

void free_letStatement(LetStatement *let_statement) {
    // Falta implementar.
}