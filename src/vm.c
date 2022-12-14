#include "common.h"
#include "vm.h"
#include "debug.h"
#include <stdio.h>
#include "compiler.h"

VM vm;

static void resetStack(){
  vm.stackTop = vm.stack;
}

void VM_init(){
  resetStack();
}

static InterpretResult run(){
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

#define BINARY_OP(op) \
  do {\
    double b = VM_pop();\
    double a = VM_pop();\
    VM_push(a op b);\
  } while(false) // do/while is just a hack to make a block in the macro syntax

  for (;;){
# ifdef DEBUG_STACK_EXECUTION
    printf("     ");
    for (Value *slot = vm.stack; slot < vm.stackTop; slot++){
      printf("[ ");
      ValueArray_print_value(*slot);
      printf(" ]");
    }
    printf("\n");

    disassembleInstruction(vm.chunk, (int) (vm.ip - vm.chunk ->code));
#endif
    uint8_t instruction;
    switch (instruction = READ_BYTE()){
      case OP_CONSTANT: {
          Value constant = READ_CONSTANT();
          VM_push(constant);
          break;
        }
      case OP_RETURN: {
         ValueArray_print_value(VM_pop());
         printf("\n");
         return INTERPRET_OK;
        }
      case OP_NEGATE: {
          VM_push(-VM_pop());
          break;
        }
      case OP_ADD: BINARY_OP(+); break;
      case OP_SUBTRACT: BINARY_OP(-); break;
      case OP_MULTIPLY: BINARY_OP(*); break;
      case OP_DIVIDE: BINARY_OP(/); break;
    }
  }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP 
}

InterpretResult VM_interpret(const char *source){
  Chunk chunk; 
  Chunk_init(&chunk);

  if (!compile(source, &chunk)){
    Chunk_free(&chunk);
    return INTERPRET_COMPILE_ERROR;
  }

  vm.chunk = &chunk;

  vm.ip = vm.chunk->code;
  InterpretResult result = run();
  Chunk_free(&chunk);
  return result;
}

void VM_push(Value value){
  *vm.stackTop = value;
  vm.stackTop++;
}

Value VM_pop(){
  vm.stackTop--;
  return *vm.stackTop;
}

void VM_free(){

}



