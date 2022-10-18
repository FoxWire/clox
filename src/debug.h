#ifndef clox_debug_h
#define clox_debug_h

#include "chunk.h"
void disassembleChunk(Chunk *chunk, const char *name);
int disassmbleInstruction(Chunk *chunk, int offset);

#endif
