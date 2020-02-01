#include "repl.h"
#include "../lib/bool.h"
#include "../parser/parser.h"
#include "../evaluator/evaluator.h"
#include <stdio.h>
#include <stdlib.h>

const char *MSG_BUG = "         \n\
        ----------------------  \n\
       |                      | \n\
       |    -YOU HAVE BUGS-   | \n\
       |                      | \n\
        ----------------------  \n\
        (  _/)    | |           \n\
        ( -.-)    | |           \n\
+/      O('')('') | |//         ";



const char *WELCOME = "\n\
Hello hobbits this is Beverly 1.0.0 alpha language.\nFeel free to write here.\n";

static char *get_line(const char *prompt);

static void print_errors(const ParserError *errors);

static void delete_data(Program *program, Lexer *lexer, Parser *parser, Object *object);

Repl new_repl(const char *prompt) {
    Repl repl = {0};
    repl._prompt = prompt;

    return repl;
}

void start_repl(Repl *repl) {
    printf("%s", WELCOME);
    char *line = NULL;
    Lexer lexer = (Lexer){0};
    Parser parser = (Parser){0};
    Program program = (Program){0};
    Object object = (Object){0};

    while (true) {
        line = get_line(repl->_prompt);
        lexer = new_lexer(line);
        parser = new_parser(&lexer);
        program = program_parser(&parser);
        if (parser.error._errors != NULL) {
            print_errors(&parser.error);
            delete_data(&program, &lexer, &parser, &object);
            continue;
        }
        object = evaluation(&program);
        if (object._obj != NULL) {
            printf("%s\n", inspect_object(&object));
        }

        // printf("%s\n", string_program(&program));

        free(line);
        line = NULL;
        delete_data(&program, &lexer, &parser, &object);
    }
    
}

static void print_errors(const ParserError *errors) {
    printf("%s\n", MSG_BUG);
    for (size_t k=0; k < errors->_len; k++) 
        printf("\t%s\n", errors->_errors[k]);
}

static void delete_data(Program *program, Lexer *lexer, Parser *parser, Object *object) {
    free_program(program);
    free_lexer(lexer);
    free_parser(parser);
    free_object(object);
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
