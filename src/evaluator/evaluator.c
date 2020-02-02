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

//----------------------------------------------------------------------------------
// Implementacion de funciones.
//----------------------------------------------------------------------------------
Object evaluation(Program *program) {
    Object result = new_object();

    for (size_t k=0; k < program->_len; k++)
        result = _eval_statement(&program->_statements[k]);

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

        default:
            //pas
            break;
    }

    return result;
}

static Object _eval_expression(Expression *expression) {
    Object result = new_object();
    switch (expression->_type) {
        case EXPR_INTEGER:
            return _eval_integer((IntegerLiteral *)expression->_ptr);
            break;
        
        case EXPR_BOOLEAN:
            return _eval_boolean((Boolean *)expression->_ptr);
            break;
        
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

    if (left->_type == OBJ_INTEGER && right->_type == OBJ_INTEGER) {
        result._obj = (OInteger *) malloc(sizeof(OInteger));
        result._type = OBJ_INTEGER;
        ((OInteger *)result._obj)->_value = 0;
        ((OInteger *)result._obj)->__string = NULL;

        int total = 0;
        if (strcmp(operator, "+") == 0)
            total = ((OInteger *)left->_obj)->_value + ((OInteger *)right->_obj)->_value; 
        
        else if (strcmp(operator, "-") == 0)
            total = ((OInteger *)left->_obj)->_value - ((OInteger *)right->_obj)->_value; 
        
        else if (strcmp(operator, "*") == 0)
            total = ((OInteger *)left->_obj)->_value * ((OInteger *)right->_obj)->_value; 

        else if (strcmp(operator, "/") == 0)
            total = ((OInteger *)left->_obj)->_value / ((OInteger *)right->_obj)->_value; 

        ((OInteger *)result._obj)->_value = total;
    }
    else {
        result._obj = (ONull *) malloc(sizeof(ONull));
        result._type = OBJ_NULL;
    }

    return result;
}
