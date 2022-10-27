#include "common.h"
#include "chunk.h"
#include "debug.h"
#include <stdio.h>
#include "vm.h"

int main(int argc, const char *argv[]){

  initVM();

  Chunk chunk;
  initChunk(&chunk);

  int one_point_two = addConstant(&chunk, 1.2);
  writeChunk(&chunk, OP_CONSTANT, 123);
  writeChunk(&chunk, one_point_two, 123);

  int three_point_four = addConstant(&chunk, 3.4);
  writeChunk(&chunk, OP_CONSTANT, 123);
  writeChunk(&chunk, three_point_four, 123);

  writeChunk(&chunk, OP_ADD, 123);

  int five_point_six = addConstant(&chunk, 5.6);
  writeChunk(&chunk, OP_CONSTANT, 123);
  writeChunk(&chunk, five_point_six, 123);

  writeChunk(&chunk, OP_DIVIDE, 123);

  writeChunk(&chunk, OP_NEGATE, 123);
  writeChunk(&chunk, OP_RETURN, 123);
  disassembleChunk(&chunk, "test chunk");

  interpret(&chunk);

  freeVM();
  freeChunk(&chunk);
  return 0;
}
