#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "compiler.h"
#include "scanner.h"
#include "parse_rules.h"

typedef struct {
  Token current;
  Token previous;
  bool had_error;
  bool panic_mode;
} Parser;

Parser parser;

Chunk *compiling_chunk;

// Error handling //

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

// Iterating through token stream //

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

// Emitting bytecode // 

static Chunk* current_chunk(){
  // this is just to help us change the code later
  return compiling_chunk;
}

static void emit_byte(uint8_t byte){
  Chunk_write(current_chunk(), byte, parser.previous.line);
}

static void emit_bytes(uint8_t byte_a, uint8_t byte_b){
  emit_byte(byte_a);
  emit_byte(byte_b);
}

static void emit_return() {
  emit_byte(OP_RETURN);
}

static void end_compiler() {
  emit_return();
}

// Parsing //

void parse(Precedence current_precedence){
  ParseFn prefix = get_rule(parser.current.type)->prefix;

  if (prefix == NULL){
    printf("!! syntax error !!\n");
  }
  prefix(parser.current);

  // at this point you have advanced
  Precedence prev_precedence = get_rule(parser.current.type)->precedence;

  while (parser.current.type != TOKEN_EOF 
      && current_precedence <= prev_precedence
      && parser.current.type != TOKEN_RIGHT_PAREN){

    ParseRule *rule = get_rule(parser.current.type);
    ParseFn infix = rule->infix;
    infix(parser.current);
  }
}

/*
 * Upon returning, each parse function must
 * ensure that all of its tokens are consumed.
 */
void number(Token token){
  double value = strtod(token.start, NULL);

  emit_byte(OP_CONSTANT);
  emit_byte(Chunk_add_constant(compiling_chunk, value));
  advance();
}

void grouping(Token token){
  consume(TOKEN_LEFT_PAREN, "Expecting left paren");
  parse(PREC_NONE);
  consume(TOKEN_RIGHT_PAREN, "Expecting right paren");
}

void unary(Token token){
  // NOTE: remember this could also be a bang
  consume(TOKEN_MINUS, "Expecting minus");
  parse(PREC_UNARY);
  emit_byte(OP_NEGATE);
  advance();
}

// 5 + 2 * 3 + 1

void binary(Token token){

  ParseRule *rule = get_rule(token.type);
  Precedence prec = rule->precedence;

  advance();

  parse(prec);
  switch(token.type){
    case TOKEN_PLUS: emit_byte(OP_ADD); break;
    case TOKEN_MINUS: emit_byte(OP_SUBTRACT); break;
    case TOKEN_STAR: emit_byte(OP_MULTIPLY); break;
    case TOKEN_SLASH: emit_byte(OP_DIVIDE); break;
    default: emit_return(); // todo syntax error?
  }
}

// Entry point // 

bool compile(const char *source, Chunk *chunk){
  init_scanner(source);

  compiling_chunk = chunk;
  parser.had_error = false;
  parser.panic_mode = false;

  advance();
  parse(PREC_NONE);
  consume(TOKEN_EOF, "Expect end of file expression");

  end_compiler();

  return !parser.had_error;
}
