#ifndef clox_vm_h
#define clox_vm_h

#include "value.h"
#include "chunk.h"

#define STACK_MAX 256

typedef struct {
  Chunk *chunk;
  uint8_t *ip;
  Value stack[STACK_MAX];
  Value *stackTop;
} VM;

typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR

}InterpretResult;


void VM_init();
void VM_free();
InterpretResult VM_interpret(const char *source);
void VM_push(Value value);
Value VM_pop();

#endif
