#include "./lib.h"
#include <stdlib.h>
#include <string.h>

char *copy_string(const char *string) {
    char *nstring = (char *) malloc(sizeof(char) * strlen(string) + 1);
    if (nstring != NULL) strcpy(nstring, string);

    return nstring;
}
