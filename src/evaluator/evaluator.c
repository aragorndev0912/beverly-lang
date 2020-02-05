#include "./evaluator.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//----------------------------------------------------------------------------------
// Firmas de funciones estaticas.
//----------------------------------------------------------------------------------
static Object _eval_statement(Statement *statement);

static Object _eval_expression(Expression *expression);

static Object _eval_integer(IntegerLiteral *integer_literal);

static Object _eval_boolean(Boolean *boolean);

static void _eval_prefixExpression(const char *operator, Object *right);

static void _eval_notOperatorPrefix(Object *right);

static void _eval_minusOperatorPrefix(Object *right);

static Object _eval_infixExpression(Object *left, const char *operator, Object *right);

static bool is_aritmetic_operator(const char *operator);

static bool is_boolean_operator(const char *operator);

static Object eval_integer_operator(Object *left, const char *operator, Object *right);

static Object eval_boolean_operator(Object *left, const char *operator, Object *right);

static Object _eval_ifElseExpression(IfExpression *_if);

static Object _eval_returnStatement(ReturnStatement *return_statement);

//----------------------------------------------------------------------------------
// Implementacion de funciones.
//----------------------------------------------------------------------------------
Object evaluation(Program *program) {
    Object result = new_object();

    for (size_t k=0; k < program->_len; k++) {
        result = _eval_statement(&program->_statements[k]);

        if (result._type == OBJ_RETURN) 
            return ((OReturn *)result._obj)->_value;
    }

    return result;
}


//----------------------------------------------------------------------------------
// Implementacion de funciones estaticas.
//----------------------------------------------------------------------------------
static Object _eval_statement(Statement *statement) {
    Object result = new_object();    
    switch (statement->_type) {
        case TYPE_EXPR_STMT:
            result = _eval_expression(&((ExpressionStatement *)statement->_ptr)->_expression);
            break;

        case TYPE_RETURN:
            result = _eval_returnStatement((ReturnStatement *)statement->_ptr);
            break;

        default:
            //pas
            break;
    }

    return result;
}

static Object _eval_returnStatement(ReturnStatement *return_statement) {
    Object result = new_object();
    result._type = OBJ_RETURN;
    result._obj = (OReturn *) malloc(sizeof(OReturn));
    ((OReturn *)result._obj)->_value = _eval_expression(&return_statement->_value);

    return result;
}


static Object _eval_expression(Expression *expression) {
    Object result = new_object();
    switch (expression->_type) {
        case EXPR_INTEGER:
            return _eval_integer((IntegerLiteral *)expression->_ptr);
        
        case EXPR_BOOLEAN:
            return _eval_boolean((Boolean *)expression->_ptr);
        
        case EXPR_PREFIX:
            ; // permite la declaracion de una statement despues de una etiqueta.
            PrefixExpression *prefix = (PrefixExpression *)expression->_ptr;
            Object p_right = _eval_expression(&prefix->_right);
            _eval_prefixExpression(prefix->_operator, &p_right);
            return p_right;

        case EXPR_INFIX:
            ;
            InfixExpression *infix = (InfixExpression *)expression->_ptr;
            Object left = _eval_expression(&infix->_left);
            Object i_right = _eval_expression(&infix->_right);
            const char *operator = infix->_operator;
            result = _eval_infixExpression(&left, operator, &i_right);
            free_object(&left);
            free_object(&i_right);

            return result;

        case EXPR_IF:
            return _eval_ifElseExpression((IfExpression *)expression->_ptr);

        default:
            //pass
            break;
    }

    return result;
}

static Object _eval_integer(IntegerLiteral *integer_literal) {
    Object object = new_object();
    object._type = OBJ_INTEGER;
    object._obj = (OInteger *) malloc(sizeof(OInteger));
    
    ((OInteger *)object._obj)->_value = integer_literal->_value;
    ((OInteger *)object._obj)->__string = NULL;
    
    return object;
}

static Object _eval_boolean(Boolean *boolean) {
    Object object = new_object();
    object._type = OBJ_BOOLEAN;
    object._obj = (OBoolean *) malloc(sizeof(OBoolean));
    
    ((OBoolean *)object._obj)->_value = boolean->_value;
    
    return object;
}

static void _eval_prefixExpression(const char *operator, Object *right) {
    // operator 'not'
    if (strcmp(operator, BEV_NOT) == 0) 
        _eval_notOperatorPrefix(right);
    
    else if (strcmp(operator, BEV_MINUS) == 0)
        _eval_minusOperatorPrefix(right);
}

static void _eval_minusOperatorPrefix(Object *right) {
    if (right->_type != OBJ_INTEGER) {
        free(right->_obj);
        right->_type = OBJ_NULL;
        right->_obj = (ONull*) malloc(sizeof(ONull));
        return;
    }

    ((OInteger *)right->_obj)->_value *= -1;
}

static void _eval_notOperatorPrefix(Object *right) {
    switch (right->_type) {
        case OBJ_BOOLEAN:
            ((OBoolean *)right->_obj)->_value = !((OBoolean *)right->_obj)->_value;
            break;
        
        case OBJ_INTEGER:
            free(right->_obj);
            right->_obj = (OBoolean *) malloc(sizeof(OBoolean));
            right->_type = OBJ_BOOLEAN;
            ((OBoolean *)right->_obj)->_value = false;
            break;

        default:
            free(right->_obj);
            right->_obj = (ONull *) malloc(sizeof(ONull));
            right->_type = OBJ_NULL;
            break;
    }
}

static Object _eval_infixExpression(Object *left, const char *operator, Object *right) {
    Object result = new_object();

    if (left->_type == OBJ_INTEGER && right->_type == OBJ_INTEGER && is_aritmetic_operator(operator)) 
        return eval_integer_operator(left, operator, right);

    else if (is_boolean_operator(operator))
        return eval_boolean_operator(left, operator, right);

    else {
        result._obj = (ONull *) malloc(sizeof(ONull));
        result._type = OBJ_NULL;
    }

    return result;
}

static bool is_aritmetic_operator(const char *operator) {
    bool result = (strcmp(operator, BEV_PLUS) == 0 || strcmp(operator, BEV_MINUS) == 0 ||
                strcmp(operator, BEV_MULT) == 0 || strcmp(operator, BEV_DIV) == 0);

    return result;
}

static bool is_boolean_operator(const char *operator) {
    return (strcmp(operator, BEV_EQUAL) == 0 || strcmp(operator, BEV_NOT_EQUAL) == 0 ||
            strcmp(operator, BEV_LT) == 0 ||strcmp(operator, BEV_GT) == 0);
}

static Object eval_integer_operator(Object *left, const char *operator, Object *right) {
    Object result = new_object();
    result._obj = (OInteger *) malloc(sizeof(OInteger));
    result._type = OBJ_INTEGER;
    ((OInteger *)result._obj)->_value = 0;
    ((OInteger *)result._obj)->__string = NULL;

    int total = 0;
    if (strcmp(operator, BEV_PLUS) == 0)
        total = ((OInteger *)left->_obj)->_value + ((OInteger *)right->_obj)->_value; 
    
    else if (strcmp(operator, BEV_MINUS) == 0)
        total = ((OInteger *)left->_obj)->_value - ((OInteger *)right->_obj)->_value; 
    
    else if (strcmp(operator, BEV_MULT) == 0)
        total = ((OInteger *)left->_obj)->_value * ((OInteger *)right->_obj)->_value; 

    else if (strcmp(operator, BEV_DIV) == 0)
        total = ((OInteger *)left->_obj)->_value / ((OInteger *)right->_obj)->_value; 

    ((OInteger *)result._obj)->_value = total;

    return result;
}

static Object eval_boolean_operator(Object *left, const char *operator, Object *right) {
    Object result = new_object();
    result._obj = (OBoolean *) malloc(sizeof(OBoolean));
    result._type = OBJ_BOOLEAN;
    ((OBoolean *)result._obj)->_value = false;

    // La primera validacion se hace de acuerdo a los tipos
    // de datos.
    if (left->_type != right->_type)
        return result;


    bool value = false;
    if (strcmp(operator, BEV_LT) == 0)
        value = ((OBoolean *)left->_obj)->_value < ((OBoolean *)right->_obj)->_value;  

    else if (strcmp(operator, BEV_GT) == 0)
        value = ((OBoolean *)left->_obj)->_value > ((OBoolean *)right->_obj)->_value;  

    else if (strcmp(operator, BEV_EQUAL) == 0)
        value = ((OBoolean *)left->_obj)->_value == ((OBoolean *)right->_obj)->_value;  

    else if (strcmp(operator, BEV_NOT_EQUAL) == 0)
        value = ((OBoolean *)left->_obj)->_value != ((OBoolean *)right->_obj)->_value;  

    ((OBoolean *)result._obj)->_value = value;

    return result;
}

static Object _eval_ifElseExpression(IfExpression *_if) {
    Object result = new_object();
    result._obj = (ONull *) malloc(sizeof(ONull));
    result._type = OBJ_NULL;

    Object condition = _eval_expression(&_if->_condition);
    if (condition._type != OBJ_BOOLEAN) {
        free_object(&condition);
        return result;
    }

    if (((OBoolean *)condition._obj)->_value && _if->_if_consequence._statements != NULL) {
        free_object(&result);
        return _eval_statement(_if->_if_consequence._statements);
    }
    else if (_if->_else_consequence._statements != NULL) {
        free_object(&result);
        return _eval_statement(_if->_else_consequence._statements);
    } 
    
    free_object(&condition);
    return result;
}
