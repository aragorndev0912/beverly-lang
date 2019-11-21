#include <stdio.h>
#include "includes/object.h"

int main(void) {

    Object obj = init_object(5);
    printf("id: %d", obj._id);

    return 0;
}