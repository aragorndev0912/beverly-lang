#include "repl.h"
#include "bool.h"
#include <stdio.h>
#include <stdlib.h>

static char *get_line(const char *prompt);

Repl new_repl(const char *prompt) {
    Repl repl = {0};
    repl._prompt = prompt;

    return repl;
}

void start_repl(Repl *repl) {
    char *line = NULL;
    while (true) {
        line = get_line(repl->_prompt);
        printf("%s\n", line);
        
        
        free(line);
        line = NULL;
    }
    
}

void free_repl(Repl *repl) {
    // Falta implementacion.
}

char *get_line(const char *prompt) {
    int diff = 4;
    int index = 0;
    char ch = 0;
    size_t max = diff;

    char *line = (char *) malloc(sizeof(char) * max);
    printf("%s ", prompt);
    while (true) {
        ch = fgetc(stdin);
        if (ch == EOF) break;

        if (index >= max) {
            max += diff;
            char *aux_line = (char *) realloc(line, max);
            if (aux_line == NULL) {
                free(line);
                line = NULL;
                return NULL;
            }

            line = aux_line;
        }

        line[index] = ch;
        if (line[index] == '\n') break;
        index += 1;
    }

    line[index] = '\0';
    return line;
}