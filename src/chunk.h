#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
  OP_RETURN,
  OP_CONSTANT,
  OP_NEGATE,
  OP_ADD,
  OP_SUBTRACT,
  OP_MULTIPLY,
  OP_DIVIDE
} OpCode;

typedef struct {
  int count;
  int capacity;
  int *lines;
  uint8_t *code;
  ValueArray constants;
} Chunk;

void Chunk_init(Chunk *chunk);
void Chunk_write(Chunk *chunk, uint8_t byte, int line);
int Chunk_add_constant(Chunk *chunk, Value value);
void Chunk_free(Chunk *chunk);

#endif
