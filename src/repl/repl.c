#include "repl.h"
#include "../lib/bool.h"
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
    Lexer lexer = {0};
    Token token = {0};

    while (true) {
        line = get_line(repl->_prompt);
        // printf("%s\n", line);
        lexer = new_lexer(line);

        token = next_token_lexer(&lexer);
        while (token._type != NULL && token._type != BEV_EOF) {
            printf("type: %s literal: %s\n", token._type, token._literal);    

            free_token(&token);
            token = next_token_lexer(&lexer);
        }
        
        if (token._type == BEV_EOF)
            free_token(&token);

        free(line);
        line = NULL;

        free_lexer(&lexer);
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
    if (line == NULL) {
        // Falta implementar limpieza.
    }
    printf("\n%s ", prompt);
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
