#include "src/repl/repl.h"
#include <stdlib.h>

int main(void) {

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    system("cls");
#elif defined(__linux__)
    system("clear");
#endif

    Repl repl = new_repl(">> ");
    start_repl(&repl);
    free_repl(&repl);

    return 0;
}
