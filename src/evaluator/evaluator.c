#include "./evaluator.h"
#include <stdlib.h>
#include <stdio.h>

//----------------------------------------------------------------------------------
// Firmas de funciones estaticas.
//----------------------------------------------------------------------------------
Object _eval_statement(Statement *statement);

Object _eval_expression(Expression *expression);

Object _eval_integer(IntegerLiteral *integer_literal);


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
Object _eval_statement(Statement *statement) {
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

Object _eval_expression(Expression *expression) {
    Object result = new_object();
    switch (expression->_type)
    {
        case EXPR_INTEGER:
            return _eval_integer(((IntegerLiteral *)expression->_ptr));
            break;
        
        default:
            //pass
            break;
    }

    return result;
}

Object _eval_integer(IntegerLiteral *integer_literal) {
    Object object = new_object();
    object._type = OBJ_INTEGER;
    object._obj = (OInteger *) malloc(sizeof(OInteger));
    
    ((OInteger *)object._obj)->_value = integer_literal->_value;
    ((OInteger *)object._obj)->__string = NULL;
    
    return object;
}
