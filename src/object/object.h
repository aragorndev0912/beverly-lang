#ifndef _OBJECT_H
#define _OBJECT_H

#include "../lib/bool.h"

//----------------------------------------------------------------------------------
// struct Object.
//----------------------------------------------------------------------------------
typedef enum TypeObject {
    OBJ_UNDEFINED = 0,
    OBJ_NULL,
    OBJ_INTEGER,
    OBJ_BOOLEAN,
    OBJ_RETURN,

} TypeObject;

typedef struct Object {
    void *_obj;
    TypeObject _type;

    char *__string;

} Object;

Object new_object(void);

void free_object(Object * object);

const char *inspect_object(Object *object);

//----------------------------------------------------------------------------------
// struct OInteger.
//----------------------------------------------------------------------------------
typedef struct OInteger {
    int _value; // entero de 4 bytes.

    char *__string;

} OInteger;

const char *inspect_ointeger(OInteger *integer);

void free_ointger(OInteger *integer);

//----------------------------------------------------------------------------------
// struct OBoolean.
//----------------------------------------------------------------------------------
typedef struct OBoolean {
    bool _value;
    TypeObject _type;

} OBoolean;

const char *inspect_oboolean(OBoolean *boolean);


//----------------------------------------------------------------------------------
// struct ONull.
//----------------------------------------------------------------------------------
typedef struct ONull {
    TypeObject _type;

} ONull;

const char *inspect_onull(ONull *null);


//----------------------------------------------------------------------------------
// struct OReturn.
//----------------------------------------------------------------------------------
typedef struct OReturn {
    Object _value;

} OReturn;

const char *inspect_oreturn(OReturn *oreturn);

void free_oreturn(OReturn *oreturn);

#endif //_OBJECT_H
