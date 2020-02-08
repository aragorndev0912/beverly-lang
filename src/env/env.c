#include "./env.h"
#include "../lib/lib.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

const size_t SIZE_TABLE = 10;

static Symbol new_symbol(const char *key, Object *value);

static Object copy_object(const Object *object);

Enviroment new_enviroment(void) {
    Enviroment enviroment = (Enviroment){0};
    enviroment._cap = 0;
    enviroment._len = 0;
    enviroment._table = NULL;

    return enviroment;
}

const Object get_object_enviroment(const Enviroment *enviroment, const char *key) {
    Object result = new_object();
    for (size_t k=0; enviroment->_table != NULL && k < enviroment->_len; k++) {
        if (strcmp(enviroment->_table[k].key, key) == 0) {
            return enviroment->_table[k].value;
        }
    }
    // La variable no esta almacenada.
    // se devuelve una estructura OError.
    result._type = OBJ_NULL;
    result._obj = (ONull *) malloc(sizeof(ONull));

    return result;
}

void set_object_enviroment(Enviroment *enviroment, const char *key, Object *value) {
    if (enviroment->_table == NULL) {
        enviroment->_len = 0;
        enviroment->_cap = SIZE_TABLE;
        enviroment->_table = (Symbol *) malloc(sizeof(Symbol) * enviroment->_cap);        
        enviroment->_table[enviroment->_len++] = new_symbol(key, value);
    }
    else {
        if (enviroment->_len >= enviroment->_cap) {
            enviroment->_cap += SIZE_TABLE;
            Symbol *aux = (Symbol *) realloc(enviroment->_table, enviroment->_cap);
            if (aux == NULL) {
                printf("ERRRRRROOOOR\n");
                return;
            }

            enviroment->_table = aux;
        }
        enviroment->_table[enviroment->_len++] = new_symbol(key, value);
    }
}

void free_enviroment(Enviroment *enviroment) {
    if (enviroment->_table != NULL) {
        for (size_t k=0; k < enviroment->_len; k++) {
            free_object(&enviroment->_table[k].value);
            free(enviroment->_table[k].key);
            enviroment->_table[k].key = NULL;
        }    
        
        free(enviroment->_table);
        enviroment->_table = NULL;
    }
}

static Symbol new_symbol(const char *key, Object *value) {
    Symbol symbol = (Symbol){.key=copy_string(key), .value=copy_object(value)};
    // printf("key:%s, value:%d\n", symbol.key, ((OInteger *)symbol.value._obj)->_value);

    return symbol;
}

static Object copy_object(const Object *object) {
    Object result = new_object();
    result.__in_table = true;

    switch (object->_type)
    {
    case OBJ_INTEGER:
        result._type = OBJ_INTEGER;
        result._obj = (OInteger *) malloc(sizeof(OInteger));
        ((OInteger *)result._obj)->_value = ((OInteger *)object->_obj)->_value;
        ((OInteger *)result._obj)->__string = ((OInteger *)object->_obj)->__string;
        break;
    
    default:
        result._type = OBJ_NULL;
        result._obj = (ONull *) malloc(sizeof(ONull));
        break;
    }

    return result;
}