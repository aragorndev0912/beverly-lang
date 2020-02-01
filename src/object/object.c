#include "./object.h"
#include <stdlib.h>
#include <stdio.h>

//----------------------------------------------------------------------------------
// struct Object.
//----------------------------------------------------------------------------------
Object new_object(void) {
    return (Object) {
        ._obj=NULL,
        ._type=OBJ_UNDEFINED,
        .__string=NULL,
    };
}

void free_object(Object * object) {
    if (object->_obj != NULL) {
        switch (object->_type) {
            case OBJ_INTEGER:
                free_ointger((OInteger *)object->_obj);
                break;
            
            default:
                //pass
                break;
        }
    
        free(object->_obj);
        object->_obj = NULL;
    }
}

const char *inspect_object(Object *object) {
    switch (object->_type)
    {
        case OBJ_INTEGER:
            return inspect_ointeger((OInteger *)object->_obj);
            break;
        
        default:
            return " ";
            break;
    }
}

//----------------------------------------------------------------------------------
// struct OInteger.
//----------------------------------------------------------------------------------
const char *inspect_ointeger(OInteger *integer) {
    integer->__string = (char *) malloc(sizeof(integer->_value) * 8 + 1);
    sprintf(integer->__string, "%d", integer->_value);

    return integer->__string; 
}

void free_ointger(OInteger *integer) {
    if (integer->__string != NULL) {
        free(integer->__string);
        integer->__string = NULL;
    }
}


//----------------------------------------------------------------------------------
// struct OBoolean.
//----------------------------------------------------------------------------------
const char *inspect_oboolean(OBoolean *boolean) {
    return (boolean->_value) ? "true" : "false";
}

//----------------------------------------------------------------------------------
// struct ONull.
//----------------------------------------------------------------------------------
const char *inspect_onull(ONull *null) {
    return "null";
}
