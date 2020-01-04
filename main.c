#include "src/repl/repl.h"

int main(void) {
    Repl repl = new_repl(">> ");
    start_repl(&repl);
    free_repl(&repl);

    return 0;
}
