#include "./lib.h"
#include <stdlib.h>
#include <string.h>

char *copy_string(const char *string) {
    char *nstring = (char *) malloc(sizeof(char) * (strlen(string) + 1));
    if (nstring != NULL) {
        // strcpy_s(nstring, (strlen(string) + 1), string);
        memmove(nstring, string, (strlen(string) + 1));
    }

    return nstring;
}

void add_string(char **destiny, const char *origin) {
    if (origin != NULL) {
        if ((*destiny) == NULL) {
            (*destiny) = (char *) malloc(sizeof(char) * (strlen(origin) + 1));
            // strcpy_s((*destiny), (strlen(origin) + 1), origin);
            memmove((*destiny), origin, (strlen(origin) + 1));
        }
        else {
            size_t cap = strlen((*destiny)) + strlen(origin);

            char *aux_destiny = (char *) malloc(sizeof(char) * (strlen((*destiny)) + 1));
            // strcpy_s(aux_destiny, (strlen((*destiny)) + 1), (*destiny));
            memmove(aux_destiny, (*destiny), (strlen((*destiny)) + 1));
            
            free((*destiny));
            (*destiny) = NULL;
            (*destiny) = (char *) malloc(sizeof(char) * (cap + 1));
            // strcpy_s((*destiny), (cap + 1), aux_destiny);
            memmove((*destiny), aux_destiny, (cap + 1));

            free(aux_destiny);
            aux_destiny = NULL;

            // strcat_s((*destiny), (cap + 1), origin);
            strcat((*destiny), origin);
        }
    }    
}
