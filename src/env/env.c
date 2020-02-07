#include "./env.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

const size_t SIZE_TABLE = 10;

static Symbol new_symbol(const char *key, Object *value);

Enviroment new_enviroment(void) {
    Enviroment enviroment = (Enviroment){0};
    enviroment._cap = 0;
    enviroment._len = 0;
    enviroment._table = NULL;

    return enviroment;
}

const Object *get_object_enviroment(const Enviroment *enviroment, const char *key) {
    for (size_t k=0; enviroment->_table != NULL && k < enviroment->_len; k++) {
        if (strcmp(enviroment->_table[k].key, key) == 0)
            return enviroment->_table[k].value;
    }

    return NULL;
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
            free_object(enviroment->_table[k].value); 
        }    
        
        free(enviroment->_table);
        enviroment->_table = NULL;
    }
}

static Symbol new_symbol(const char *key, Object *value) {
    return (Symbol){.key=key, .value=value};
}