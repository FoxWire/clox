#include <stdio.h>
#include <string.h>
#include "dbg.h"

#include "common.h"
#include "scanner.h"
#include "scanner_utils.c"

const char *TokenTypeNames[40] = {
  "TOKEN_LEFT_PAREN", "TOKEN_RIGHT_PAREN", "TOKEN_LEFT_BRACE", "TOKEN_RIGHT_BRACE", "TOKEN_COMMA", 
   "TOKEN_DOT", "TOKEN_MINUS", "TOKEN_PLUS", "TOKEN_SEMICOLON", "TOKEN_SLASH", 
   "TOKEN_STAR", "TOKEN_BANG", "TOKEN_BANG_EQUAL", "TOKEN_EQUAL", "TOKEN_EQUAL_EQUAL",
  "TOKEN_GREATER", "TOKEN_GREATER_EQUAL", "TOKEN_LESS", "TOKEN_LESS_EQUAL", "TOKEN_IDENTIFIER", 
  "TOKEN_STRING", "TOKEN_NUMBER", "TOKEN_AND", "TOKEN_CLASS", "TOKEN_ELSE", 
  "TOKEN_FALSE", "TOKEN_FOR", "TOKEN_FUN", "TOKEN_IF", "TOKEN_NIL", 
  "TOKEN_OR", "TOKEN_PRINT", "TOKEN_RETURN", "TOKEN_SUPER", "TOKEN_THIS", 
  "TOKEN_TRUE", "TOKEN_VAR", "TOKEN_WHILE", "TOKEN_ERROR", "TOKEN_EOF"
};

void print_token_type(TokenType type){
  printf("%s\n", TokenTypeNames[type]);
}

typedef struct {
  const char *start;
  const char *current;
  int line;
} Scanner;

Scanner scanner;

void init_scanner(const char *source){
  scanner.start = source;
  scanner.current = source;
  scanner.line = 1;
}

static bool isAtEnd(){
  return *scanner.current == '\0';
}

static Token make_token(TokenType type){
  Token token;
  token.type = type;
  token.start = scanner.start;
  token.length = (int) (scanner.current - scanner.start);
  token.line = scanner.line;
  return token;
}

static Token make_error_token(const char *message){
  Token token;
  token.type = TOKEN_ERROR;
  token.start = message;
  token.length = (int) strlen(message);
  token.line = scanner.line;
  return token;
}

char advance(){
  scanner.current++;
  return scanner.current[-1];
}

static char peek(){
  return *scanner.current;
}

static bool match(char expected){
  if (isAtEnd()) return false;
  if (*scanner.current != expected) return false;
  scanner.current++;
  return true;
}

static void skip_whitespace(){

  char current = *scanner.current;
  while (is_whitespace(current)){
    if (current == '\n'){
      scanner.line++;
    }
    current = *++scanner.current;
  }
}

static void skip_comments(){

  if (scanner.current[0] == '/'
      && scanner.current[1] == '/'){
    while (*scanner.current != '\n'){
      scanner.current++;
    }
    scanner.current++; // consume newline
  }
}

static Token make_string_token(){

  const char *start_string = scanner.current;
  while (*scanner.current != '\"'){
    if (isAtEnd()){
      return make_error_token("Unterminated string");
    }
    if (*scanner.current == '\n'){
      scanner.line++;
    }

    scanner.current++;
  }
  const char *end_string = scanner.current;
  scanner.current++; // bump one to remove '"'

  Token token;
  token.type = TOKEN_STRING;
  token.start = start_string;
  token.length = (int) (end_string - start_string);
  token.line = scanner.line;
  return token;
}

static Token make_number_token(){

  while (is_numeric(*scanner.current)){
    scanner.current++;
  }

  if (*scanner.current == '.'){
    scanner.current++;
    while (is_numeric(*scanner.current)){
      scanner.current++;
    }
  }

  return make_token(TOKEN_NUMBER);
}

static TokenType match_token_type(const int start_offset, const int length_of_rest, char *rest, TokenType type){
  long calc_length = scanner.current - (scanner.start + start_offset);
  if (calc_length == length_of_rest
      && memcmp(scanner.start + start_offset, rest, length_of_rest) == 0) {
    return type;
  }

  return TOKEN_IDENTIFIER;
}

static TokenType get_token_type(){
  
  char start_char = *scanner.start;

  switch(start_char){
    case 'a': return match_token_type(1, 2, "nd", TOKEN_AND);
    case 'c': return match_token_type(1, 4, "lass", TOKEN_CLASS);
    case 'e': return match_token_type(1, 3, "lse", TOKEN_ELSE);
    case 'i': return match_token_type(1, 1, "f", TOKEN_IF);
    case 'n': return match_token_type(1, 2, "il", TOKEN_NIL);
    case 'o': return match_token_type(1, 1, "r", TOKEN_OR);
    case 'p': return match_token_type(1, 4, "rint", TOKEN_PRINT);
    case 'r': return match_token_type(1, 5, "eturn", TOKEN_RETURN);
    case 's': return match_token_type(1, 4, "uper", TOKEN_SUPER);
    case 'v': return match_token_type(1, 2, "ar", TOKEN_VAR);
    case 'w': return match_token_type(1, 4, "hile", TOKEN_WHILE);
    case 'f': 
              if (scanner.current - scanner.start > 1){
                switch (scanner.start[1]){
                  case 'a': return match_token_type(2, 3, "lse", TOKEN_FALSE);
                  case 'o': return match_token_type(2, 1, "r", TOKEN_FOR);
                  case 'u': return match_token_type(2, 1, "n", TOKEN_FUN);
                }
              }
    case 't':
              if (scanner.current - scanner.start > 1){
                switch (scanner.start[1]){
                  case 'h': return match_token_type(2, 2, "is", TOKEN_THIS);
                  case 'r': return match_token_type(2, 2, "ue", TOKEN_TRUE);
                }
              }
  }

  return TOKEN_IDENTIFIER;
}

static Token make_keyword_or_identifier(){
  while (is_alpha(*scanner.current) 
      || is_numeric(*scanner.current) 
      || *scanner.current == '_'){
    scanner.current++;
  }

  return make_token(get_token_type());
}


// MAIN ENTRY POINT //
Token scan_token(){

  skip_whitespace(); 
  skip_comments(); 

  if (isAtEnd()){
    return make_token(TOKEN_EOF);
  }

  scanner.start = scanner.current;

  char next_char = advance();

  if (is_numeric(next_char)){
    return make_number_token();
  }

  if (is_alpha(next_char)){
    return make_keyword_or_identifier();
  }

  switch (next_char) {
    case '\"': return make_string_token();
    case '(': return make_token(TOKEN_LEFT_PAREN);
    case ')': return make_token(TOKEN_RIGHT_PAREN);
    case '{': return make_token(TOKEN_LEFT_BRACE);
    case '}': return make_token(TOKEN_RIGHT_BRACE);
    case ';': return make_token(TOKEN_SEMICOLON);
    case ',': return make_token(TOKEN_COMMA);
    case '.': return make_token(TOKEN_DOT);
    case '-': return make_token(TOKEN_MINUS);
    case '+': return make_token(TOKEN_PLUS);
    case '/': return make_token(TOKEN_SLASH);
    case '*': return make_token(TOKEN_STAR);
    case '!': return make_token(match('=') ? TOKEN_BANG_EQUAL: TOKEN_BANG);
    case '=': return make_token(match('=') ? TOKEN_EQUAL_EQUAL: TOKEN_EQUAL);
    case '<': return make_token(match('=') ? TOKEN_LESS_EQUAL: TOKEN_LESS);
    case '>': return make_token(match('=') ? TOKEN_GREATER_EQUAL: TOKEN_GREATER);
  }

  return make_error_token("Unexpected character");
}
