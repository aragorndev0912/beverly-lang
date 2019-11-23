#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void) {

    char c = 'h';
    const char *str = &c;
    
    printf("c: %s\n", str);

    return 0;
}