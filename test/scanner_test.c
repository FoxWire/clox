#include "minunit.h"
#include "../src/scanner.h"

static char* extract_string(Token token){
  char *dest = malloc(token.length + 1);
  memcpy(dest, token.start, token.length);
  dest[token.length] = '\0';
  return dest;
}

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

char *string_literals_are_parsed(){

  init_scanner("(\"Hello, world!\")");

  Token first_token = scan_token();
  mu_assert(first_token.type == TOKEN_LEFT_PAREN, "should be left paren");

  Token second_token = scan_token();
  mu_assert(second_token.type == TOKEN_STRING, "");

  char *token_string = extract_string(second_token);

  mu_assert_str(token_string, "Hello, world!", "message");

  Token third_token = scan_token();
  mu_assert(third_token.type == TOKEN_RIGHT_PAREN, "expecting right paren");

  free(token_string);
  return NULL;
}

char *hitting_eof_before_termining_string_gives_unterminated_string_error(){

  init_scanner("\"Hello, world!"); 

  Token token = scan_token();
  mu_assert(token.type == TOKEN_ERROR, "expecting error token");

  char *token_string = extract_string(token);

  mu_assert_str(token_string, "Unterminated string", "");

  free(token_string);
  return NULL;
}

char *newlines_in_string_literals_increase_line_count(){

  init_scanner("\"Hello, \nworld!:\""); 

  Token token = scan_token();
  mu_assert(token.type == TOKEN_STRING, "");
  mu_assert(token.line == 2, "");

  return NULL;
}

char *integers_are_scanned(){

  init_scanner("(123)");

  Token first = scan_token();
  mu_assert(first.type == TOKEN_LEFT_PAREN, "expecting left paren");

  Token second = scan_token();
  mu_assert(second.type == TOKEN_NUMBER, "expecting token number");
  char *token_string = extract_string(second);
  mu_assert_str(token_string, "123", "expecting integer 123");

  Token third = scan_token();
  mu_assert(third.type == TOKEN_RIGHT_PAREN, "expecting right paren");

  free(token_string);
  return NULL;
}

char *doubles_are_scanned(){

  init_scanner("(123.52)");
  Token first = scan_token();
  mu_assert(first.type == TOKEN_LEFT_PAREN, "expecting left paren");

  Token second = scan_token();
  mu_assert(second.type == TOKEN_NUMBER, "expecting token number");
  char *token_string = extract_string(second);
  mu_assert_str(token_string, "123.52", "expecting double 123.52");

  Token third = scan_token();
  mu_assert(third.type == TOKEN_RIGHT_PAREN, "expecting right paren");

  return NULL;
}

char *trailing_doubles_are_scanned(){
  init_scanner("(123.)");

  Token first = scan_token();
  mu_assert(first.type == TOKEN_LEFT_PAREN, "expecting left paren");

  Token second = scan_token();
  mu_assert(second.type == TOKEN_NUMBER, "expecting token number");
  char *token_string = extract_string(second);
  mu_assert_str(token_string, "123.", "expecting trailing double 123.");

  Token third = scan_token();
  mu_assert(third.type == TOKEN_RIGHT_PAREN, "expecting right paren");

  return NULL;
}

char *identifiers_are_scanned(){

  init_scanner("my_variable123");

  Token token = scan_token();
  mu_assert(token.type == TOKEN_IDENTIFIER, "expecting identifier");
  char *token_string = extract_string(token);
  mu_assert_str(token_string, "my_variable123", "expecting 'my_variable123'");

  free(token_string);
  return NULL;
}

char *all_keywords_are_scanned(){

  init_scanner("and class else if nil or print return super var while false for fun this true");

  Token token = scan_token();
  mu_assert(token.type == TOKEN_AND, "expecting and");

  token = scan_token();
  mu_assert(token.type == TOKEN_CLASS, "expecting class");

  token = scan_token();
  mu_assert(token.type == TOKEN_ELSE, "expecting else");

  token = scan_token();
  mu_assert(token.type == TOKEN_IF, "expecting if");

  token = scan_token();
  mu_assert(token.type == TOKEN_NIL, "expecting nil");

  token = scan_token();
  mu_assert(token.type == TOKEN_OR, "expecting or");

  token = scan_token();
  mu_assert(token.type == TOKEN_PRINT, "expecting print");

  token = scan_token();
  mu_assert(token.type == TOKEN_RETURN, "expecting return");

  token = scan_token();
  mu_assert(token.type == TOKEN_SUPER, "expecting super");

  token = scan_token();
  mu_assert(token.type == TOKEN_VAR, "expecting var");

  token = scan_token();
  mu_assert(token.type == TOKEN_WHILE, "expecting while");

  token = scan_token();
  mu_assert(token.type == TOKEN_FALSE, "expecting false");

  token = scan_token();
  mu_assert(token.type == TOKEN_FOR, "expecting for");

  token = scan_token();
  mu_assert(token.type == TOKEN_FUN, "expecting fun");

  token = scan_token();
  mu_assert(token.type == TOKEN_THIS, "expecting this");

  token = scan_token();
  mu_assert(token.type == TOKEN_TRUE, "expecting true");

  return NULL;
}

char *keywords_are_scanned(){

  init_scanner("and");

  Token token = scan_token();
  mu_assert(token.type == TOKEN_AND, "expecting and");

  return NULL;
}

char *keywords_are_filtered_on_length(){

  init_scanner("ardvark");

  Token token = scan_token();
  mu_assert(token.type != TOKEN_AND, "expecting identifier");

  return NULL;
}

char *keywords_are_filtered_on_content(){

  init_scanner("are");

  Token token = scan_token();
  mu_assert(token.type != TOKEN_AND, "expecting identifier");

  return NULL;
}

char *all_tests() {

  mu_suite_start();
  mu_run_test(test_basic_scanning);
  mu_run_test(whitespace_is_ignored);
  mu_run_test(newline_in_whitespace_increases_line_count);
  mu_run_test(single_slashes_are_not_treated_as_comments);
  mu_run_test(comments_cause_rest_of_line_to_be_ignored);
  mu_run_test(string_literals_are_parsed);
  mu_run_test(hitting_eof_before_termining_string_gives_unterminated_string_error);
  mu_run_test(newlines_in_string_literals_increase_line_count);
  mu_run_test(integers_are_scanned);
  mu_run_test(doubles_are_scanned);
  mu_run_test(trailing_doubles_are_scanned);
  mu_run_test(identifiers_are_scanned);
  mu_run_test(keywords_are_scanned);
  mu_run_test(keywords_are_filtered_on_length);
  mu_run_test(keywords_are_filtered_on_content);

  return NULL;
}

RUN_TESTS(all_tests);




