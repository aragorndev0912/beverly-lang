#include "./evaluator.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../lib/lib.h"

//----------------------------------------------------------------------------------
// Firmas de funciones estaticas.
//----------------------------------------------------------------------------------
static Object _eval_statement(Statement *statement, Enviroment *enviroment);

static Object _eval_expression(Expression *expression, Enviroment *enviroment);

static Object _eval_integer(IntegerLiteral *integer_literal);

static Object _eval_boolean(Boolean *boolean);

static Object _eval_prefixExpression(const char *operator, Object *right);

static Object _eval_notOperatorPrefix(Object *right);

static Object _eval_minusOperatorPrefix(Object *right);

static Object _eval_infixExpression(Object *left, const char *operator, Object *right);

static bool is_aritmetic_operator(const char *operator);

static bool is_boolean_operator(const char *operator);

static Object eval_integer_operator(Object *left, const char *operator, Object *right);

static Object eval_boolean_operator(Object *left, const char *operator, Object *right);

static Object _eval_ifElseExpression(IfExpression *_if, Enviroment *enviroment);

static Object _eval_returnStatement(ReturnStatement *return_statement, Enviroment *enviroment);

static const char *_get_object_type(const Object *object);

static Object _eval_letStatement(LetStatement *let_statement, Enviroment *enviroment);

static Object _eval_identifier(Identifier *identifier, Enviroment *enviroment);

//----------------------------------------------------------------------------------
// Implementacion de funciones.
//----------------------------------------------------------------------------------
Object evaluation(Program *program, Enviroment *enviroment) {
    Object result = new_object();

    for (size_t k=0; k < program->_len; k++) {
        result = _eval_statement(&program->_statements[k], enviroment);

        if (result._type == OBJ_RETURN) 
            return ((OReturn *)result._obj)->_value;
        
        else if (result._type == OBJ_ERROR)
            return result;
    }
    return result;
}

void new_oerror(OError *oerror, const char *msg) {
    oerror->_value = (char *) malloc(sizeof(char) * strlen(msg) + 1);
    // memcpy(oerror->_value, msg, strlen(msg));
    strcpy(oerror->_value, msg);
}


//----------------------------------------------------------------------------------
// Implementacion de funciones estaticas.
//----------------------------------------------------------------------------------
static Object _eval_statement(Statement *statement, Enviroment *enviroment) {
    Object result = new_object();    
    switch (statement->_type) {
        case TYPE_EXPR_STMT:
            return _eval_expression(&((ExpressionStatement *)statement->_ptr)->_expression, enviroment);

        case TYPE_RETURN:
            return _eval_returnStatement((ReturnStatement *)statement->_ptr, enviroment);


        case TYPE_LET:
            return _eval_letStatement((LetStatement *)statement->_ptr, enviroment);

        default:
            //pas
            break;
    }

    return result;
}

static Object _eval_returnStatement(ReturnStatement *return_statement, Enviroment *enviroment) {
    Object result = new_object();
    result._type = OBJ_RETURN;
    result._obj = (OReturn *) malloc(sizeof(OReturn));
    ((OReturn *)result._obj)->_value = _eval_expression(&return_statement->_value, enviroment);

    return result;
}

static Object _eval_expression(Expression *expression, Enviroment *enviroment) {
    Object result = new_object();
    switch (expression->_type) {
        case EXPR_IDENTIFIER:
            return _eval_identifier((Identifier *)expression->_ptr, enviroment);

        case EXPR_INTEGER:
            return _eval_integer((IntegerLiteral *)expression->_ptr);
        
        case EXPR_BOOLEAN:
            return _eval_boolean((Boolean *)expression->_ptr);
        
        case EXPR_PREFIX:
            ; // permite la declaracion de una statement despues de una etiqueta.
            PrefixExpression *prefix = (PrefixExpression *)expression->_ptr;
            Object p_right = _eval_expression(&prefix->_right, enviroment);
            return _eval_prefixExpression(prefix->_operator, &p_right);

        case EXPR_INFIX:
            ;
            InfixExpression *infix = (InfixExpression *)expression->_ptr;
            Object left = _eval_expression(&infix->_left, enviroment);
            Object i_right = _eval_expression(&infix->_right, enviroment);
            const char *operator = infix->_operator;
            result = _eval_infixExpression(&left, operator, &i_right);
            if (!left.__in_table) free_object(&left);
            if (!i_right.__in_table) free_object(&i_right);

            return result;

        case EXPR_IF:
            return _eval_ifElseExpression((IfExpression *)expression->_ptr, enviroment);

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

static Object _eval_prefixExpression(const char *operator, Object *right) {
    // operator 'not'
    Object result = new_object();
    if (strcmp(operator, BEV_NOT) == 0) 
        return _eval_notOperatorPrefix(right);
    
    else if (strcmp(operator, BEV_MINUS) == 0)
        return _eval_minusOperatorPrefix(right);
    
    else {
        const char *_type = _get_object_type(right);
        // if(!right->__in_table) free_object(right);
        result._type = OBJ_ERROR;
        result._obj = (OError *) malloc(sizeof(OError));

        char *msg = (char *) malloc(sizeof(char) * 50);
        sprintf(msg, "unknown operator: %s %s", operator, _type);

        new_oerror((OError *)result._obj, msg);
        free(msg);
        msg = NULL;
    }

    return result;
}

static Object _eval_minusOperatorPrefix(Object *right) {
    Object result = new_object();
    if (right->_type != OBJ_INTEGER)
         {
        const char * _type = _get_object_type(right);

        result._type = OBJ_ERROR;
        result._obj = (OError *) malloc(sizeof(OError));
        char *msg = (char *) malloc(sizeof(char) * 100);
        sprintf(msg, "unknown operator: -%s", _type);
        new_oerror((OError *)result._obj, msg);
        
        free(msg);
        msg = NULL;

        return result;
    }

    result._type = OBJ_INTEGER;
    result._obj = (OInteger *) malloc(sizeof(OInteger));
    ((OInteger *)result._obj)->_value = ((OInteger *)right->_obj)->_value * -1;

    return result;
}

static Object _eval_notOperatorPrefix(Object *right) {
    Object result = new_object();
    switch (right->_type) {
        case OBJ_BOOLEAN:
            result._type = OBJ_BOOLEAN;
            result._obj = (OBoolean *) malloc(sizeof(OBoolean));
            ((OBoolean *)result._obj)->_value = !((OBoolean *)right->_obj)->_value;
            break;
        
        case OBJ_INTEGER:
            result._type = OBJ_BOOLEAN;
            result._obj = (OBoolean *) malloc(sizeof(OBoolean));
            ((OBoolean *)result._obj)->_value = false;
            break;

        default:
            right->_type = OBJ_NULL;
            right->_obj = (ONull *) malloc(sizeof(ONull));
            break;
    }

    return result;
}

static Object _eval_infixExpression(Object *left, const char *operator, Object *right) {
    Object result = new_object();

    if (left->_type == OBJ_INTEGER && right->_type == OBJ_INTEGER && is_aritmetic_operator(operator)) 
        return eval_integer_operator(left, operator, right);

    else if (is_boolean_operator(operator))
        return eval_boolean_operator(left, operator, right);

    else if (left->_type != right->_type) {
        result._type = OBJ_ERROR;
        result._obj = (OError *) malloc(sizeof(OError));

        char *msg = (char *)malloc(sizeof(char) * 100);
        sprintf(msg, "type mismatch: %s %s %s", _get_object_type(left), operator, _get_object_type(right));
        new_oerror((OError *)result._obj, msg);
        free(msg);
        msg = NULL;
    }
    else {
        // result._obj = (ONull *) malloc(sizeof(ONull));
        // result._type = OBJ_NULL;
        result._type = OBJ_ERROR;
        result._obj = (OError *) malloc(sizeof(OError));

        char *msg = (char *)malloc(sizeof(char) * 100);
        sprintf(msg, "unknown operator: %s %s %s", _get_object_type(left), operator, _get_object_type(right));
        new_oerror((OError *)result._obj, msg);
        free(msg);
        msg = NULL;
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

static Object _eval_ifElseExpression(IfExpression *_if, Enviroment *enviroment) {
    Object result = new_object();
    result._obj = (ONull *) malloc(sizeof(ONull));
    result._type = OBJ_NULL;

    Object condition = _eval_expression(&_if->_condition, enviroment);
    if (condition._type != OBJ_BOOLEAN) {
        if(!condition.__in_table) free_object(&condition);
        return result;
    }

    if (((OBoolean *)condition._obj)->_value && _if->_if_consequence._statements != NULL) {
        free_object(&result);
        return _eval_statement(_if->_if_consequence._statements, enviroment);
    }
    else if (_if->_else_consequence._statements != NULL) {
        free_object(&result);
        return _eval_statement(_if->_else_consequence._statements, enviroment);
    } 
    
    if(!condition.__in_table) free_object(&condition);
    return result;
}

static const char *_get_object_type(const Object *object) {
    switch (object->_type)
    {
    case OBJ_INTEGER:
        return "INTEGER";

    case OBJ_BOOLEAN:
        return "BOOLEAN";

    case OBJ_NULL:
        return "NULL";
    
    case OBJ_RETURN:
        return "RETURN";

    case OBJ_ERROR:
        return "ERROR";

    default:
        return "UNDEFINED";
    }
}

static Object _eval_letStatement(LetStatement *let_statement, Enviroment *enviroment) {
    Object result = _eval_expression(&let_statement->_value, enviroment);
    if (result._type == OBJ_NULL)
        return result;
    
    set_object_enviroment(enviroment, let_statement->_name._value, &result);
    return result;
}

static Object _eval_identifier(Identifier *identifier, Enviroment *enviroment) {
    Object result = get_object_enviroment(enviroment, identifier->_value); 
    if (result._type == OBJ_NULL) {
        free_object(&result);

        result._type = OBJ_ERROR;
        result._obj = (OError *) malloc(sizeof(OError));

        char *msg = (char *) malloc(sizeof(char) * 100);
        sprintf(msg, "identifier not found: %s", identifier->_value);
        new_oerror((OError *)result._obj, msg);
        
        free(msg);
        msg = NULL;
    }

    return result; 
}