#include "./lib.h"
#include <stdlib.h>
#include <string.h>

char *copy_string(const char *string) {
    char *nstring = (char *) malloc(sizeof(char) * strlen(string) + 1);
    if (nstring != NULL) strcpy(nstring, string);

    return nstring;
}

void add_string(const char **destiny, const char *origin) {
    if (destiny == NULL) {
        // I need to implement it.

    }
    else {
        // I need to implement it.
    }
}