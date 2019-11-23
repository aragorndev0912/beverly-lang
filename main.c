#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void) {

    char *str0 = "hello sds";
    char *str1 = "hello";

    printf("equal: %d\n", str0 == str1);

    return 0;
}