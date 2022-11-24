#include "minunit.h"
#include "../src/scanner.h"

char *test_basic_scanning(){

  initScanner("{*};");
  mu_assert(scanToken().type == TOKEN_LEFT_BRACE, 
      "Expecting left brace but got right brace");
  mu_assert(scanToken().type == TOKEN_STAR, 
      "Expecting left brace but got right brace");
  mu_assert(scanToken().type == TOKEN_RIGHT_BRACE, 
      "Expecting left brace but got right brace");
  mu_assert(scanToken().type == TOKEN_SEMICOLON, 
      "Expecting left brace but got right brace");

  return NULL;
}

char *whitespace_is_ignored(){
  initScanner(" \t\r\n;");
  mu_assert(scanToken().type == TOKEN_SEMICOLON, 
      "Expecting spaces to be ignored");

  return NULL;
}

char *newline_in_whitespace_increases_line_count(){
  initScanner("; \t\r\n;");
  Token first_token = scanToken();
  mu_assert(first_token.line == 1 ,"should be still on first line")
  Token second_token = scanToken();
  mu_assert(second_token.line == 2 ,"should have passed newline");

  return NULL;
}


char *all_tests()
{
  mu_suite_start();
  mu_run_test(test_basic_scanning);
  mu_run_test(whitespace_is_ignored);
  mu_run_test(newline_in_whitespace_increases_line_count);

  return NULL;
}

RUN_TESTS(all_tests);




