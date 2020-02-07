#ifndef _EVALUATOR_H
#define _EVALUATOR_H

#include "../object/object.h"
#include "../ast/ast.h"
#include "../env/env.h"

Object evaluation(Program *program, Enviroment *enviroment);

void new_oerror(OError *oerror, const char *msg);

#endif //_EVALUATOR_H