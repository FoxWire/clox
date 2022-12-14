#include <stdlib.h>
#include <stdio.h>

#include "chunk.h"
#include "memory.h"

void Chunk_init(Chunk *chunk){
  chunk->capacity = 0;
  chunk->count = 0;
  chunk->code = NULL;
  chunk->lines = NULL;
  ValueArray_init(&chunk->constants);
}

void Chunk_write(Chunk *chunk, uint8_t byte, int line){
  if (chunk->capacity < chunk->count + 1){
    int old_capacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(old_capacity);
    chunk->code = GROW_ARRAY(uint8_t, chunk->code, old_capacity, chunk->capacity);
    chunk->lines = GROW_ARRAY(int, chunk->lines, old_capacity, chunk->capacity);
  }

  chunk->code[chunk->count] = byte;
  chunk->lines[chunk->count] = line;
  chunk->count++;
}

int Chunk_add_constant(Chunk *chunk, Value value){
  ValueArray_write_value(&chunk->constants, value);
  return chunk->constants.count - 1;
}

void Chunk_free(Chunk *chunk){
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(int, chunk->lines, chunk->capacity);
  ValueArray_free(&chunk->constants);
  Chunk_init(chunk);
}

int addition(int a, int b){
  return a + b;
}







