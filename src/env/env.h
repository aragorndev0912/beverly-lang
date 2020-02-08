#ifndef _ENV_H
#define _ENV_H

#include "stddef.h"
#include "../object/object.h"

//----------------------------------------------------------------------------------
// StructEnviroment.
//----------------------------------------------------------------------------------
typedef struct Symbol {
    char *key;
    Object value;

} Symbol;

typedef struct Enviroment {
    Symbol *_table;
    size_t _cap;
    size_t _len;

} Enviroment;

Enviroment new_enviroment(void);

const Object get_object_enviroment(const Enviroment *enviroment, const char *key);

void set_object_enviroment(Enviroment *enviroment, const char *key, Object *value);

void free_enviroment(Enviroment *enviroment);

#endif //_ENV