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

void initScanner(const char *source){
  scanner.start = source;
  scanner.current = source;
  scanner.line = 1;
}

static bool isAtEnd(){
  return *scanner.current == '\0';
}

static Token makeToken(TokenType type){
  Token token;
  token.type = type;
  token.start = scanner.current;
  token.length = (int) (scanner.current - scanner.start);
  token.line = scanner.line;
  return token;
}

static Token errorToken(const char *message){
  Token token;
  token.type = TOKEN_ERROR;
  token.start = message;
  token.length = (int)strlen(message);
  token.line = scanner.line;
  return token;
}

char advance(){
  scanner.current++;
  return scanner.current[-1];
}

static bool match(char expected){
  if (isAtEnd()) return false;
  if (*scanner.current != expected) return false;
  scanner.current++;
  return true;
}

static void skipWhiteSpace(){
  char current = *scanner.current;
  while( current == ' '
      || current == '\t'
      || current == '\r'){
    current = advance();
  }
}


Token scanToken(){
  // hello
  skipWhiteSpace();

  scanner.start = scanner.current;

  if (isAtEnd()){
    return makeToken(TOKEN_EOF);
  }


 char c = advance();

  switch (c) {
    case '(': return makeToken(TOKEN_LEFT_PAREN);
    case ')': return makeToken(TOKEN_RIGHT_PAREN);
    case '{': return makeToken(TOKEN_LEFT_BRACE);
    case '}': return makeToken(TOKEN_RIGHT_BRACE);
    case ';': return makeToken(TOKEN_SEMICOLON);
    case ',': return makeToken(TOKEN_COMMA);
    case '.': return makeToken(TOKEN_DOT);
    case '-': return makeToken(TOKEN_MINUS);
    case '+': return makeToken(TOKEN_PLUS);
    case '/': return makeToken(TOKEN_SLASH);
    case '*': return makeToken(TOKEN_STAR);
    case '!': return makeToken(match('=') ? TOKEN_BANG_EQUAL: TOKEN_BANG);
    case '=': return makeToken(match('=') ? TOKEN_EQUAL_EQUAL: TOKEN_EQUAL);
    case '<': return makeToken(match('=') ? TOKEN_LESS_EQUAL: TOKEN_LESS);
    case '>': return makeToken(match('=') ? TOKEN_GREATER_EQUAL: TOKEN_GREATER);
  }

  return errorToken("Unexpected character");



}
