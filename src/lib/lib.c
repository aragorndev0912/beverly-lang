#include "./lib.h"
#include <stdlib.h>
#include <string.h>

char *copy_string(const char *string, size_t n) {
    char *nstring = (char *) malloc(sizeof(char) * n + 1);
    strcpy(nstring, string);

    return nstring;
}
