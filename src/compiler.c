#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "compiler.h"
#include "scanner.h"

typedef struct {
  Token current;
  Token previous;
  bool had_error;
  bool panic_mode;
} Parser;

Parser parser;

static void error_at(Token *token, const char *message){

  if (parser.panic_mode){
    return;
  }

  fprintf(stderr, "[line %d] Error", token->line);

  if (token->type == EOF){
    fprintf(stderr, " at end");
  } else if (token->type == TOKEN_ERROR){
    // Nothing
  } else {
    fprintf(stderr, " at '%.*s'", token->length, token->start);
  }

  fprintf(stderr, ": %s\n", message);
  parser.had_error = true;
}

static void error(const char *message){
  error_at(&parser.previous, message);
}

static void error_at_current(const char *message){
  error_at(&parser.current, message);
}

static void advance(){
  parser.previous = parser.current;
  while (true){
    parser.current = scan_token();
    if (parser.current.type != TOKEN_ERROR){
      break;
    }
    error_at_current(parser.current.start);
  }
}

static void consume(TokenType token_type, const char *message){
  if (parser.current.type == token_type){
    advance();
    return;
  }
  error_at_current(message);
}





bool compile(const char *source, Chunk *chunk){
  init_scanner(source);

  parser.had_error = false;
  parser.panic_mode = false;

  advance();
  expression();
  consume(TOKEN_EOF, "Expect end of file expression");

  return !parser.had_error;
}
