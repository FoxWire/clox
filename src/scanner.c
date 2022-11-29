#include <stdio.h>
#include <string.h>
#include "dbg.h"

#include "common.h"
#include "scanner.h"

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
  token.start = scanner.current;
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

static bool is_whitespace(const char character){
  return (character == ' '
      || character == '\t'
      || character == '\r'
      || character == '\n');
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

static bool is_numeric(char character){
  return (character >= '0' && character <= '9');
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

  Token token;
  token.type = TOKEN_NUMBER;
  token.start = scanner.start; 
  token.length = (int) (scanner.current - scanner.start);
  token.line = scanner.line;
  return token;
}

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
