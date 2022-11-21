#include "minunit.h"
#include "../src/scanner.h"

char *test_basic_scanning(){

//  initScanner("{*};");
 // mu_assert(scanToken().type == TOKEN_LEFT_BRACE, 
  //    "Expecting left brace but got right brace");
  //mu_assert(scanToken().type == TOKEN_STAR, 
      //"Expecting left brace but got right brace");
  //mu_assert(scanToken().type == TOKEN_RIGHT_BRACE, 
      //"Expecting left brace but got right brace");
  //mu_assert(scanToken().type == TOKEN_SEMICOLON, 
      //"Expecting left brace but got right brace");

  return NULL;
}

char *whitespace_is_ignored(){

  //initScanner(" \t\r\n;");
  //mu_assert(scanToken().type == TOKEN_SEMICOLON, 
      //"Expecting spaces to be ignored");

  return NULL;
}

char *all_tests()
{
  mu_suite_start();
  mu_run_test(test_basic_scanning);
  mu_run_test(whitespace_is_ignored);

  return NULL;
}

RUN_TESTS(all_tests);




