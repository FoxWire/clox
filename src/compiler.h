#ifndef clox_compiler_h
#define clox_compiler_h

#include "vm.h"
#include "scanner.h"


bool compile(const char *source, Chunk *chunk);

void number(Token token);
void unary(Token token);
void grouping(Token token);
void binary(Token token);


#endif
