#include "minunit.h"
#include "../src/scanner.h"

char *test_basic_scanning(){

  init_scanner("{*};");
  mu_assert(scan_token().type == TOKEN_LEFT_BRACE, "Expecting left brace but got right brace");
  mu_assert(scan_token().type == TOKEN_STAR, "Expecting left brace but got right brace");
  mu_assert(scan_token().type == TOKEN_RIGHT_BRACE, "Expecting left brace but got right brace");
  mu_assert(scan_token().type == TOKEN_SEMICOLON, "Expecting left brace but got right brace");

  return NULL;
}

char *whitespace_is_ignored(){
  init_scanner(" \t\r\n;");
  mu_assert(scan_token().type == TOKEN_SEMICOLON, "Expecting spaces to be ignored");

  return NULL;
}

char *newline_in_whitespace_increases_line_count(){
  init_scanner("; \t\r\n;");

  Token first_token = scan_token();
  mu_assert(first_token.line == 1 ,"should be still on first line")

  Token second_token = scan_token();
  mu_assert(second_token.line == 2 ,"should have passed newline");

  return NULL;
}

char *single_slashes_are_not_treated_as_comments(){
  init_scanner("*/!");

  Token first_token = scan_token();
  mu_assert(first_token.type == TOKEN_STAR, "should be asterisk");

  Token second_token = scan_token();
  mu_assert(second_token.type == TOKEN_SLASH, "should be slash");

  Token third_token = scan_token();
  mu_assert(third_token.type == TOKEN_BANG, "should be bang");

  return NULL;
}



char *comments_cause_rest_of_line_to_be_ignored(){
  init_scanner("*//!\n*");

  Token first_token = scan_token();
  mu_assert(first_token.type == TOKEN_STAR, "should be asterisk");

  Token second_token = scan_token();
  mu_assert(second_token.type == TOKEN_STAR, "should have skipped '!'");

  return NULL;
}


char *all_tests()
{
  mu_suite_start();
  mu_run_test(test_basic_scanning);
  mu_run_test(whitespace_is_ignored);
  mu_run_test(newline_in_whitespace_increases_line_count);
  mu_run_test(single_slashes_are_not_treated_as_comments);
  mu_run_test(comments_cause_rest_of_line_to_be_ignored);

  return NULL;
}

RUN_TESTS(all_tests);




