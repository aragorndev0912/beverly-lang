#include "repl.h"
#include "bool.h"
#include <stdio.h>

// char * getline(void) {
//     char * line = malloc(100), * linep = line;
//     size_t lenmax = 100, len = lenmax;
//     int c;

//     if(line == NULL)
//         return NULL;

//     for(;;) {
//         c = fgetc(stdin);
//         if(c == EOF)
//             break;

//         if(--len == 0) {
//             len = lenmax;
//             char * linen = realloc(linep, lenmax *= 2);

//             if(linen == NULL) {
//                 free(linep);
//                 return NULL;
//             }
//             line = linen + (line - linep);
//             linep = linen;
//         }

//         if((*line++ = c) == '\n')
//             break;
//     }
//     *line = '\0';
//     return linep;
// }

Repl new_repl(const char *prompt) {
    Repl repl = {0};
    repl._prompt = prompt;

    return repl;
}

void start_repl(Repl *repl) {
    char data[1024];
    while (true) {
        printf("%s", repl->_prompt);
        scanf("%s", data);
        printf("data: %s\n",data);
    }
}

void free_repl(Repl *repl) {
    // Falta implementacion.
}