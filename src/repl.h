#ifndef BEV_REPL
#define BEV_REPL

#include "token.h"
#include "lexer.h"

typedef struct Repl
{
    const char *_prompt;

} Repl;


Repl new_repl(const char *prompt);

void start_repl(Repl *repl);

void free_repl(Repl *repl);

#endif //BEV_REPL