#include "minunit.h"
#include "../src/compiler.h"

//static char* extract_string(Token token){
  //char *dest = malloc(token.length + 1);
  //memcpy(dest, token.start, token.length);
  //dest[token.length] = '\0';
  //return dest;
//}

char *test_compile_number_literals(){

  Chunk chunk;
  Chunk_init(&chunk);

  char *source = "2.0";

  bool success = compile(source, &chunk);

  mu_assert(success == true, "should compile without error");

  mu_assert(chunk.code[0] == OP_CONSTANT, "should have emitted OP_CONSTANT");
  // Value value = chunk.constants.values[chunk.code[1]];

  //mu_assert(value == 2.0, "should put value in value table");


  return NULL;
}

char *all_tests() {

  mu_suite_start();
  mu_run_test(test_compile_number_literals);

  return NULL;
}

RUN_TESTS(all_tests);




