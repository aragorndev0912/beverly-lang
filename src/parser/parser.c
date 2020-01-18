#include "parser.h"
#include "../lib/bool.h"
#include "../lib/lib.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//----------------------------------------------------------------------------------
// Declaracion de constantes.
//----------------------------------------------------------------------------------

static const size_t PARSER_ERROR_SIZE = 10;

//----------------------------------------------------------------------------------
// Declaracion de funciones estaticas.
//----------------------------------------------------------------------------------
static bool is_current_token(const Parser *const parser, const char *token);
static bool is_peek_token(const Parser *const parser, const char *token);
static bool expect_token(Parser *parser, const char *token);

static Statement exprStmt_parser(Parser *parser);
static Statement letStmt_parser(Parser *parser);
static Statement returnStmt_parser(Parser *parser);

static Expression expression_parser(Parser *parser, Precedence pre);
static Precedence token_precedence(const Token *token);

static Expression new_infix_expression(Parser *parser, const Expression *left);

static BlockStatement new_block_statement(Parser *parser);
//----------------------------------------------------------------------------------
// Implementacion de funciones ParserError.
//----------------------------------------------------------------------------------
ParserError new_parsererror(void) {
    ParserError p_error;
    p_error._errors = NULL;
    p_error._len = 0;
    p_error._cap = 0;

    return p_error;
}

void free_parsererror(ParserError *p_error) {
    if (p_error->_errors != NULL) {
        for (int k=0; k < p_error->_len; k++) {
            free(p_error->_errors[k]);
            p_error->_errors[k] = NULL;
        }

        free(p_error->_errors);
        p_error->_errors = NULL;
    }
}

void add_parsererror(ParserError *p_error, const char *error) {
    if (p_error->_errors == NULL) {
        p_error->_cap = PARSER_ERROR_SIZE;
        p_error->_errors = (char **) malloc(sizeof(char *) * p_error->_cap);
        if (p_error->_errors == NULL) {
            // I need to implement it.
        }
        p_error->_errors[p_error->_len++] = copy_string(error);
    }
    else {
        if (p_error->_len >= p_error->_cap) {
            p_error->_cap += PARSER_ERROR_SIZE; 
            char **_tmp = (char **) realloc(p_error->_errors, p_error->_cap);
            if (_tmp == NULL) {
                // Falta implementacion.
            }
            p_error->_errors = _tmp;
            _tmp = NULL;
        }

        p_error->_errors[p_error->_len++] = copy_string(error);
    }
}

//----------------------------------------------------------------------------------
// Implementacion de funciones Parser.
//----------------------------------------------------------------------------------
Parser new_parser(Lexer *lexer) {
    Parser parser = {0};
    parser._lexer = lexer;
    // Inicializar token sin definicion
    parser._current_token = (Token) {._type=BEV_UNDEFINED, ._literal=NULL};
    parser._peek_token = (Token) {._type=BEV_UNDEFINED, ._literal=NULL};

    next_token_parser(&parser);
    next_token_parser(&parser);

    parser.error = new_parsererror();

    return parser;
}

void free_parser(Parser *parser) {
    if (parser != NULL) {
        if (parser->_lexer != NULL) {
            // free_lexer(parser->_lexer);
            parser->_lexer = NULL;
        }

        if (parser->error._errors != NULL) {
            for (int k=0; k < parser->error._len; k++) {
                free(parser->error._errors[k]);
                parser->error._errors[k] = NULL;
            }
        }   
    }
}

void next_token_parser(Parser *parser) {
    if (parser->_current_token._type != BEV_UNDEFINED) 
        free_token(&parser->_current_token);

    parser->_current_token = parser->_peek_token;
    parser->_peek_token = next_token_lexer(&(*parser->_lexer));
}

Program program_parser(Parser *parser) {
    Program program = new_program();
    program._statements = NULL;

    Statement stmt = (Statement) {0};
    while (parser->_current_token._type != BEV_EOF) {
        stmt = stmt_parser(parser);
        // Ocurrio algun error en la construccion del AST.
        if (stmt._ptr != NULL)
            add_stmt_program(&program, stmt._ptr, stmt._type);

        next_token_parser(parser);
    }   

    free_token(&parser->_current_token);
    free_token(&parser->_peek_token);

    return program;
}

Statement stmt_parser(Parser *parser) {
    Statement stmt = (Statement) {._ptr=NULL, ._type=TYPE_FAILURE};
    if (parser->_current_token._type == BEV_LET) 
        return letStmt_parser(parser);
    else if (parser->_current_token._type == BEV_RETURN)
        return  returnStmt_parser(parser);
    else 
        return exprStmt_parser(parser);

    return stmt;
}

static Statement exprStmt_parser(Parser *parser) {
    Statement stmt = (Statement) {._ptr=NULL, ._type=TYPE_FAILURE};
    stmt._ptr = (ExpressionStatement *) malloc(sizeof(ExpressionStatement)); 
    if (stmt._ptr == NULL) {
        // I need to implement it.
    }
    stmt._type = TYPE_EXPR_STMT;

    ((ExpressionStatement *)stmt._ptr)->_token = new_token(
        parser->_current_token._type,
        copy_string(parser->_current_token._literal)
    );

    ((ExpressionStatement *)stmt._ptr)->_expression = expression_parser(parser, LOWEST);
    ((ExpressionStatement *)stmt._ptr)->__string = NULL;

    if (is_peek_token(parser, BEV_SEMICOLON))
        next_token_parser(parser);

    return stmt;
}

static Statement returnStmt_parser(Parser *parser) {
    Statement stmt = (Statement) {0};
    stmt._type = TYPE_RETURN;
    
    stmt._ptr = (ReturnStatement *) malloc(sizeof(ReturnStatement));
    if (stmt._ptr == NULL) {
        // I need to implement it.
    }
    ((ReturnStatement *)stmt._ptr)->_token = new_token(
        parser->_current_token._type, 
        copy_string(parser->_current_token._literal)
    );
    ((ReturnStatement *)stmt._ptr)->__string = NULL;
    
    // Inicializo la Expresion con valores nulos.
    ((ReturnStatement *)stmt._ptr)->_value = (Expression) {0};

    next_token_parser(parser);

    while (!is_current_token(parser, BEV_SEMICOLON)) 
        next_token_parser(parser);

    return stmt;
}

static Statement letStmt_parser(Parser *parser) {
    Statement stmt = (Statement) {0};
    stmt._type = TYPE_LET;

    // Reservo memoria para una estructura de tipo LetStatement.
    stmt._ptr = (LetStatement *) malloc(sizeof(LetStatement));
    if (stmt._ptr == NULL) {
        // I need to implement it.
    }

    ((LetStatement *)stmt._ptr)->_token = new_token(parser->_current_token._type, copy_string(parser->_current_token._literal));
    ((LetStatement *)stmt._ptr)->__string = NULL;

    // Si el proximo token es diferente a un BEV_IDENT
    // genero un error (FAILURE).
    if (!expect_token(parser, BEV_IDENT)) {
        // Elimina la memoria reservada del token.
        free_token(&((LetStatement *)stmt._ptr)->_token);
        return (Statement) {._ptr=NULL, ._type=TYPE_FAILURE};;
    }

    // Inicializo el identificador.
    ((LetStatement *)stmt._ptr)->_name = new_identifier(&parser->_current_token, parser->_current_token._literal);

    if (!expect_token(parser, BEV_ASSIGN)) {
        // Elimina la memoria reservada del token.
        free_token(&((LetStatement *)stmt._ptr)->_token);
        return (Statement) {._ptr=NULL, ._type=TYPE_FAILURE};;
    }

    // Por ahora no se examina las expresiones.
    while (!is_current_token(parser, BEV_SEMICOLON)) 
        next_token_parser(parser);
    
    return stmt;
}

void peek_error_parser(Parser *parser, const char *token) {
    char msg[200];
    sprintf(msg, "expected next token to be %s, got '%s' instead\n", token, parser->_peek_token._literal);

    add_parsererror(&parser->error, msg);
}


//----------------------------------------------------------------------------------
// Implementacion de funciones estaticas.
//----------------------------------------------------------------------------------
static Precedence token_precedence(const Token *token) {
    if (strcmp(token->_type, BEV_EQUAL) == 0)
        return EQUALS;
    else if (strcmp(token->_type, BEV_NOT_EQUAL) == 0)
        return EQUALS;
    else if (strcmp(token->_type, BEV_LT) == 0)
        return LESSGREATER;     
    else if (strcmp(token->_type, BEV_GT) == 0)
        return LESSGREATER;
    else if (strcmp(token->_type, BEV_PLUS) == 0)
        return SUM;
    else if (strcmp(token->_type, BEV_MINUS) == 0)
        return SUM;     
    else if (strcmp(token->_type, BEV_DIV) == 0)
        return PRODUCT;
    else if (strcmp(token->_type, BEV_MULT) == 0)
        return PRODUCT;
    else if (strcmp(token->_type, BEV_LPAREN) == 0)
        return CALL;
    
    return LOWEST;
}

static Expression expression_parser(Parser *parser, Precedence pre) {
    Expression expression = (Expression) {._ptr=NULL, ._type=EXPR_FAILURE, .__string=NULL};

    // Opciones de prefix.
    if (strcmp(parser->_current_token._type, BEV_IDENT) == 0) {
        expression._ptr = (Identifier *) malloc(sizeof(Identifier));
        if (expression._ptr == NULL) {
            // Falta implementar.
        }
        // Inicializacion de Identifier.
        ((Identifier *)expression._ptr)->_token = new_token(parser->_current_token._type, copy_string(parser->_current_token._literal));
        ((Identifier *)expression._ptr)->_value = copy_string(parser->_current_token._literal);
        expression._type = EXPR_IDENTIFIER;
    }
    else if (strcmp(parser->_current_token._type, BEV_INT) == 0) {
        expression._ptr = (IntegerLiteral *) malloc(sizeof(IntegerLiteral));
        if (expression._ptr == NULL) {
            // Falta implementar
        }


        ((IntegerLiteral *)expression._ptr)->_token = new_token(parser->_current_token._type, copy_string(parser->_current_token._literal));
        // ((IntegerLiteral *)expression._ptr)->_value = atoll(parser->_current_token._literal);
        ((IntegerLiteral *)expression._ptr)->_value = atoi(parser->_current_token._literal);
        expression._type = EXPR_INTEGER;
    }
    else if (strcmp(parser->_current_token._type, BEV_FALSE) == 0 || strcmp(parser->_current_token._type, BEV_TRUE) == 0) {
        expression._ptr = (Boolean *) malloc(sizeof(Boolean));
        if (expression._ptr == NULL) {
            // Falta implementar.
        }

        // Inicializo Boolean.
        ((Boolean *)expression._ptr)->_token = new_token(parser->_current_token._type, copy_string(parser->_current_token._literal));
        ((Boolean *)expression._ptr)->_value = (strcmp(parser->_current_token._type, BEV_TRUE) == 0) ? true : false;
        expression._type = EXPR_BOOLEAN;
    }
    else if (strcmp(parser->_current_token._type, BEV_NOT) == 0 || strcmp(parser->_current_token._type, BEV_MINUS) == 0) {
        expression._ptr = (PrefixExpression *) malloc(sizeof(PrefixExpression));
        if (expression._ptr == NULL) {
            // Falta implementar.
        }

        // Inicializo PrefixExpression.
        ((PrefixExpression *)expression._ptr)->_token = new_token(parser->_current_token._type, copy_string(parser->_current_token._literal));
        ((PrefixExpression *)expression._ptr)->_operator = copy_string(parser->_current_token._literal);
        ((PrefixExpression *)expression._ptr)->__string = NULL;

        next_token_parser(parser);
        ((PrefixExpression *)expression._ptr)->_right = expression_parser(parser, PREFIX);
        expression._type = EXPR_PREFIX;
    }
    else if (strcmp(parser->_current_token._type, BEV_LPAREN) == 0) {
        next_token_parser(parser); 

        expression = expression_parser(parser, LOWEST);
        if (!expect_token(parser, BEV_RPAREN)) {
            printf("Error in group_expression_parser.\n");
            return (Expression) {._ptr=NULL, ._type=EXPR_FAILURE, .__string=NULL};
        }
    }
    else if (strcmp(parser->_current_token._type, BEV_IF) == 0) {
        expression._ptr = (IfExpression *) malloc(sizeof(IfExpression));
        if (expression._ptr == NULL) {
            printf("Error al reservar memoria para IfExpression.\n");
        }
        expression._type = EXPR_IF;

        ((IfExpression *)expression._ptr)->__string = NULL;
        ((IfExpression *)expression._ptr)->_token = new_token(parser->_current_token._type, copy_string(parser->_current_token._literal));
        
        if (!expect_token(parser, BEV_LPAREN)) {
            return (Expression) {._ptr=NULL, ._type=EXPR_FAILURE, .__string=NULL};
        }
        next_token_parser(parser);
        ((IfExpression *)expression._ptr)->_condition = expression_parser(parser, LOWEST);
        if (!expect_token(parser, BEV_RPAREN)) {
            return (Expression) {._ptr=NULL, ._type=EXPR_FAILURE, .__string=NULL};
        }
        if (!expect_token(parser, BEV_LBRACE)) {
            return (Expression) {._ptr=NULL, ._type=EXPR_FAILURE, .__string=NULL};
        }
        ((IfExpression *)expression._ptr)->_if_consequence = new_block_statement(parser);

        if (is_peek_token(parser, BEV_ELSE)) {
            next_token_parser(parser);

            if (!expect_token(parser, BEV_LBRACE)) {
                return (Expression) {._ptr=NULL, ._type=EXPR_FAILURE, .__string=NULL};
            }

            ((IfExpression *)expression._ptr)->_else_consequence = new_block_statement(parser);
        }
    }

    // Validar expresion.
    if (expression._ptr == NULL) {
        // Agregar error.
        return expression;
    }

    while (strcmp(parser->_peek_token._type, BEV_SEMICOLON) != 0 && pre < token_precedence(&parser->_peek_token)) {
        next_token_parser(parser);
        expression = new_infix_expression(parser, &expression);
    }

    return expression;
}

static BlockStatement new_block_statement(Parser *parser) {
    BlockStatement block = (BlockStatement) {._statements=NULL, ._len=0, ._cap=0, .__string=NULL};
    block._token = new_token(parser->_current_token._type, copy_string(parser->_current_token._literal));
    next_token_parser(parser);
    while (!is_current_token(parser, BEV_RBRACE)) {
        Statement statement = stmt_parser(parser);
        if (statement._ptr != NULL)
            add_element_block_statement(&block, statement);
        
        next_token_parser(parser);
    }

    return block;
}


static Expression new_infix_expression(Parser *parser, const Expression *left) {
    Expression expression = (Expression) {._ptr=NULL, ._type=EXPR_FAILURE, .__string=NULL};

    // Inicializo Expression.
    expression._ptr = (InfixExpression *) malloc(sizeof(InfixExpression));
    expression.__string = NULL;
    expression._type = EXPR_INFIX;

    // Inicializo el contenido de Expression.
    ((InfixExpression *)expression._ptr)->__string = NULL;
    ((InfixExpression *)expression._ptr)->_left = (*left);
    ((InfixExpression *)expression._ptr)->_operator = copy_string(parser->_current_token._literal);
    ((InfixExpression *)expression._ptr)->_token = new_token(parser->_current_token._type, copy_string(parser->_current_token._literal));

    Precedence pre = token_precedence(&parser->_current_token);
    next_token_parser(parser);
    ((InfixExpression *)expression._ptr)->_right = expression_parser(parser, pre);

    return expression;
}

static bool is_current_token(const Parser *const parser, const char *token) {
    return (strcmp(parser->_current_token._type, token) == 0);
}

static bool is_peek_token(const Parser *const parser, const char *token) {
    return (strcmp(parser->_peek_token._type, token) == 0);
}

static bool expect_token(Parser *parser, const char *token) {
    if (is_peek_token(parser, token)) {
        next_token_parser(parser);
        return true;
    }

    peek_error_parser(parser, token);
    return false;
}
