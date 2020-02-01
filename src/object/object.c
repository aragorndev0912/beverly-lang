#include "./object.h"
#include <stdlib.h>
#include <stdio.h>

//----------------------------------------------------------------------------------
// struct Object.
//----------------------------------------------------------------------------------
void free_object(Object * object) {
    if (object->_obj != NULL) {
        switch (object->_type) {
            case OBJ_INTEGER:
                break;
            
            default:
                //pass
                break;
        }
    
        free(object->_obj);
        object->_obj = NULL;
    }
}


//----------------------------------------------------------------------------------
// struct OInteger.
//----------------------------------------------------------------------------------
const char *insinspect_ointegerpect(OInteger *integer) {
    char buffer[sizeof(integer->_value) * 8 + 1];
    sprintf(buffer, "%d", integer->_value);

    return "buffer"; //OJO
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
