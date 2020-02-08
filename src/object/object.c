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
        .__in_table=false,
    };
}

void free_object(Object * object) {
    if (object->_obj != NULL) {
        switch (object->_type) {
            case OBJ_INTEGER:
                // printf("value: %d\n", ((OInteger *)object->_obj)->_value);
                free_ointger((OInteger *)object->_obj);
                break;
            
            case OBJ_RETURN:
                free_oreturn((OReturn *)object->_obj);
            
            case OBJ_ERROR:
                free_oerror((OError *)object->_obj);

            default:
                //pass
                break;
        }
    
        free(object->_obj);
        object->_obj = NULL;
    }
}

const char *inspect_object(Object *object) {
    switch (object->_type) {
        case OBJ_INTEGER:
            return inspect_ointeger((OInteger *)object->_obj);
            break;
        
        case OBJ_BOOLEAN:
            return inspect_oboolean((OBoolean *)object->_obj);

        case OBJ_NULL:
            return inspect_onull((ONull *)object->_obj);
        
        case OBJ_ERROR:
            return inspect_oerror((OError *)object->_obj);

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


//----------------------------------------------------------------------------------
// struct OReturn.
//----------------------------------------------------------------------------------
const char *inspect_oreturn(OReturn *oreturn) {
    return inspect_object(&oreturn->_value);
}

void free_oreturn(OReturn *oreturn) {
    // falta implementar.   
}

//----------------------------------------------------------------------------------
// struct OError.
//----------------------------------------------------------------------------------
const char *inspect_oerror(OError *oerror) {
    return oerror->_value;
}

void free_oerror(OError *oerror) {
    if (oerror->_value != NULL) {
        free(oerror->_value);
        oerror->_value = NULL;
    }
}