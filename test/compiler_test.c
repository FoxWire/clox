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
  Value value = chunk.constants.values[chunk.code[1]];

  mu_assert(value == 2.0, "should put value in value table");

  mu_assert(chunk.count == 3, "should only be three bytes");
  mu_assert(chunk.code[2] == OP_RETURN, "should end with return");

  Chunk_free(&chunk);
  return NULL;
}

char *test_compile_binary_expression(){

  Chunk chunk;
  Chunk_init(&chunk);

  char *source = "2 + 1";

  bool success = compile(source, &chunk);
  mu_assert(success == true, "should compile without error");

  // 2
  mu_assert(chunk.code[0] == OP_CONSTANT, "should have emitted OP_CONSTANT");
  Value first_value = chunk.constants.values[chunk.code[1]];
  mu_assert(first_value == 2, "should put value in value table");
  
  // 1
  mu_assert(chunk.code[2] == OP_CONSTANT, "should have emitted OP_CONSTANT");
  Value second_value = chunk.constants.values[chunk.code[3]];
  mu_assert(second_value == 1, "should put value in value table");

  // + 
  mu_assert(chunk.code[4] == OP_ADD, "expecting OP_ADD");

  mu_assert(chunk.code[5] == OP_RETURN, "should end with return");
  mu_assert(chunk.count == 6, "should only be 6 bytes");

  Chunk_free(&chunk);
  return NULL;
}

char *test_compile_binary_expression_with_associativity(){

  Chunk chunk;
  Chunk_init(&chunk);

  char *source = "2 * 5 * 4 * 3";

  bool success = compile(source, &chunk);
  mu_assert(success == true, "should compile without error");

  // 2
  mu_assert(chunk.code[0] == OP_CONSTANT, "should have emitted OP_CONSTANT");
  Value first_value = chunk.constants.values[chunk.code[1]];
  mu_assert(first_value == 2, "should put value in value table");
  
  // 5
  mu_assert(chunk.code[2] == OP_CONSTANT, "should have emitted OP_CONSTANT");
  Value second_value = chunk.constants.values[chunk.code[3]];
  mu_assert(second_value == 5, "should put value in value table");

  // 4
  mu_assert(chunk.code[4] == OP_CONSTANT, "should have emitted OP_CONSTANT");
  Value third_value = chunk.constants.values[chunk.code[5]];
  mu_assert(third_value == 4, "should put value in value table");

  // 3
  mu_assert(chunk.code[6] == OP_CONSTANT, "should have emitted OP_CONSTANT");
  Value fourth_value = chunk.constants.values[chunk.code[7]];
  mu_assert(fourth_value == 3, "should put value in value table");

  // * 
  mu_assert(chunk.code[8] == OP_MULTIPLY, "expecting OP_MULTIPLY");
  mu_assert(chunk.code[9] == OP_MULTIPLY, "expecting OP_MULTIPLY");
  mu_assert(chunk.code[10] == OP_MULTIPLY, "expecting OP_MULTIPLY");

  mu_assert(chunk.code[11] == OP_RETURN, "should end with return");
  mu_assert(chunk.count == 12, "should be 12 bytes");

  Chunk_free(&chunk);
  return NULL;
}

char *test_compile_expression_grouping_with_parens() {
  Chunk chunk;
  Chunk_init(&chunk);

  char *source = "(1 + 2) * 3";

  bool success = compile(source, &chunk);
  mu_assert(success == true, "should compile without error");

  // 1
  mu_assert(chunk.code[0] == OP_CONSTANT, "should have emitted OP_CONSTANT");
  Value first_value = chunk.constants.values[chunk.code[1]];
  mu_assert(first_value == 1, "should put value in value table");
  
  // 2
  mu_assert(chunk.code[2] == OP_CONSTANT, "should have emitted OP_CONSTANT");
  Value second_value = chunk.constants.values[chunk.code[3]];
  mu_assert(second_value == 2, "should put value in value table");
  
  // +
  mu_assert(chunk.code[4] == OP_ADD, "expecting OP_MULTIPLY");

  // 3
  mu_assert(chunk.code[5] == OP_CONSTANT, "should have emitted OP_CONSTANT");
  Value third_value = chunk.constants.values[chunk.code[6]];
  mu_assert(third_value == 3, "should put value in value table");

  // *
  mu_assert(chunk.code[7] == OP_MULTIPLY, "expecting OP_MULTIPLY");

  mu_assert(chunk.code[8] == OP_RETURN, "should end with return");
  mu_assert(chunk.count == 9, "should be 9 bytes");

  Chunk_free(&chunk);
  return NULL;
}

char *test_compile_expression_with_negation() {
  Chunk chunk;
  Chunk_init(&chunk);

  char *source = "-5";

  bool success = compile(source, &chunk);
  mu_assert(success == true, "should compile without error");

  // 5
  mu_assert(chunk.code[0] == OP_CONSTANT, "should have emitted OP_CONSTANT");
  Value first_value = chunk.constants.values[chunk.code[1]];
  mu_assert(first_value == 5, "should put value in value table");

  // -
  mu_assert(chunk.code[2] == OP_NEGATE, "expecting OP_NEGATE");
  
  mu_assert(chunk.code[3] == OP_RETURN, "should end with return");
  mu_assert(chunk.count == 4, "should be 9 bytes");

  Chunk_free(&chunk);
  return NULL;
}

char *all_tests() {

  mu_suite_start();
  mu_run_test(test_compile_number_literals);
  mu_run_test(test_compile_binary_expression);
  mu_run_test(test_compile_binary_expression_with_associativity);
  mu_run_test(test_compile_expression_grouping_with_parens);
  mu_run_test(test_compile_expression_with_negation);

  return NULL;
}

RUN_TESTS(all_tests);




