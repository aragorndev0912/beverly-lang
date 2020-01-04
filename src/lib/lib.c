#include "./lib.h"
#include <stdlib.h>
#include <string.h>

char *copy_string(const char *string) {
    char *nstring = (char *) malloc(sizeof(char) * strlen(string) + 1);
    if (nstring != NULL) 
        strcpy(nstring, string);

    return nstring;
}

void add_string(char **destiny, const char *origin) {
    if ((*destiny) == NULL) {
        (*destiny) = (char *) malloc(sizeof(char) * (strlen(origin) + 1));
        strcpy((*destiny), origin);
    }
    else {
        size_t cap = strlen((*destiny)) + strlen(origin);

        char *aux_destiny = (char *) malloc(sizeof(char) * (strlen((*destiny)) + 1));
        strcpy(aux_destiny, (*destiny));
        
        free((*destiny));
        (*destiny) = NULL;
        (*destiny) = (char *) malloc(sizeof(char) * (cap + 1));
        strcpy((*destiny), aux_destiny);

        free(aux_destiny);
        aux_destiny = NULL;

        strcat((*destiny), origin);
    }
}
